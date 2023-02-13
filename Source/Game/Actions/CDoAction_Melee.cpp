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

void ACDoAction_Melee::End_DoAction()
{
	// 콤보 연결 실패 or 막타
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	
	ComboCount = 0;
	DisableCombo();
	bSucceed = false;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);

	int32 before = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);

	// HitStop
	if (FMath::IsNearlyZero(Datas[ComboCount].HitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2e-2f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", 2e-2f * Datas[ComboCount].HitStop, false);
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
	TSubclassOf<UCameraShake> shake = Datas[ComboCount].ShakeClass;
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
		InOtherCharacter->TakeDamage(Datas[ComboCount].Power, e, InAttacker->GetController(), InCauser);
	}
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{	
}

void ACDoAction_Melee::RestoreGlobalTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

