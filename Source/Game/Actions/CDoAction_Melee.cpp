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

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
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
		OwnerCharacter->PlayAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
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
	OwnerCharacter->PlayAnimMontage(StrongData.AnimMontage, StrongData.PlayRate, StrongData.StartSection);
	StrongData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::EndDoStrongAction()
{
	Super::EndDoStrongAction();

	IsStrongAction = true;	// 후속타 나가는중임

	State->SetEndingStrongActionMode();
	OwnerCharacter->PlayAnimMontage(StrongData.AnimMontage, StrongData.PlayRate, StrongData.EndSection);
	StrongData.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::CheckEndDoStrongAction()
{
	Super::CheckEndDoStrongAction();
	if (NextEndStrongAction)
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
			InOtherCharacter->TakeDamage(StrongData.Power, e, InAttacker->GetController(), InCauser);
		}
		else // 일반공격 피해 주기
		{
			InOtherCharacter->TakeDamage(Datas[ComboCount].Power, e, InAttacker->GetController(), InCauser);
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

