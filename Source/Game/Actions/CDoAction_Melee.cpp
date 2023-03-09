#include "CDoAction_Melee.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);
	if (bCanCombo == true)
	{
		bCanCombo = false;
		bSucceed = true;
	}
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	IsStrongAction = false;

	PlayAttackAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bSucceed);
	bSucceed = false;

	HittedCharacters.Empty();

	OwnerCharacter->StopAnimMontage();	// 몽타주 안에서도 타수 끊어버리기 (한 몽타주 타수 나누기)

	ComboCount++;
	if (ComboCount != Datas.Num())
	{
		PlayAttackAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
		Datas[ComboCount].bCanMove ? Status->SetMove() : Status->SetStop();
	}
	else
		End_DoAction();
}
void ACDoAction_Melee::DoStrongAction()
{
	Super::DoStrongAction();

	IsStrongAction = true;
	NextEndStrongAction = false;	// 일단은 false 로 

	State->SetStrongActionMode();
	PlayAttackAnimMontage(StrongData.AnimMontage, StrongData.PlayRate, StrongData.StartSection);
	StrongData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::EndDoStrongAction()
{
	Super::EndDoStrongAction();

	IsStrongAction = true;	// 후속타 나가는중임

	State->SetEndingStrongActionMode();
	PlayAttackAnimMontage(StrongData.AnimMontage, StrongData.PlayRate, StrongData.EndSection);
	StrongData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::CheckEndDoStrongAction()
{
	Super::CheckEndDoStrongAction();
	if (NextEndStrongAction || StrongData.EndType == EStrongActionEndType::Once)	// 직접 끄거나, Once인 경우 호출
	{
		EndDoStrongAction();
	}
}

void ACDoAction_Melee::EndDoStrongActionWait()
{
	Super::EndDoStrongActionWait();
	// 노티파이 체크용 변수를 true로 만든다
	NextEndStrongAction = true;
}

void ACDoAction_Melee::End_DoAction()
{
	// 콤보 연결 실패 or 막타
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	
	ComboCount = 0;
	DisableCombo();

	bSucceed = false;
	IsStrongAction = false; 
	NextEndStrongAction = false;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAim()
{
	if(State->IsIdleMode())	// IDle 모드일때만 가드하기
	{ 
		Status->SetSpeed(EWalkSpeedType::Walk);		
		State->SetGuardMode();
		//Todo: 어떤 가드 상태에 대한 ParryCheck Timeline Start
	}
}

void ACDoAction_Melee::OffAim()
{
	if (State->IsGuardMode())
	{
		Status->SetSpeed(EWalkSpeedType::Run);
		State->SetIdleMode();	// 가드모드일때만 가드풀기
	}
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	int32 before = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	TSubclassOf<UCameraShake> shake;

	// HitStop
	if (IsStrongAction == true)
	{
		// 강공격 HitStop
		if (FMath::IsNearlyZero(StrongData.HitStop) == false)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
			UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", 2e-2f * StrongData.HitStop, false);
		}
		// 강공격 CameraShake
		shake = StrongData.ShakeClass;
	}
	else
	{	// 일반공격 HitStop
		if (FMath::IsNearlyZero(Datas[ComboCount].HitStop) == false)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
			UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", 2e-2f * Datas[ComboCount].HitStop, false);
		}
		// 일반공격 CameraShake
		shake = Datas[ComboCount].ShakeClass;
	}


	//Play Particle
	UParticleSystem* hitEffect = Datas[ComboCount].Effect;
	if(!!hitEffect)
	{
		FTransform transform = Datas[ComboCount].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	//Camera Shake
	// Strong여부는 위에서 설정
	if (!!shake)
	{
		APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (!!controller)	// 설마 없진 않...으려나?
		{
			controller->PlayerCameraManager->PlayCameraShake(shake);
		}
	}

	// 배열 크기가 달라졌을 때 데미지 부여 (다단히트 방지: 무려 내가 제안한 방식)
	if (before < HittedCharacters.Num())
	{
		FDamageEvent e;
		if (IsStrongAction == true)	//StrongAction 피해 주기
		{
			SendDamage(StrongData.Power, e, InAttacker, InCauser, InOtherCharacter);
		}
		else // 일반공격 피해 주기
		{
			SendDamage(Datas[ComboCount].Power, e, InAttacker, InCauser, InOtherCharacter);
		}
	}
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{	
}

void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

