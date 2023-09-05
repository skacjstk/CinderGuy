#include "CDoAction.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Components/CAttachmentStatusComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"

// Sets default values
ACDoAction::ACDoAction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

void ACDoAction::SetParryDamageType(TSubclassOf<UDamageType> InDamageTypeClass)
{
	ParryDamageTypeClass = InDamageTypeClass;
}

bool ACDoAction::GetEquippedFromEquipment()
{
	// MyIndex가 없으면 항상 False 반환
	if (OwnerCharacter == nullptr || MyIndex == -1)
		return false;
	UCActionComponent* ActionComp =	CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	if (!!ActionComp)
	{
		UCActionObjectContainer* DO = ActionComp->GetDataObject(MyIndex);
		if (!!DO)
		{
			ACEquipment* Equipment = DO->GetEquipment();
			if (!!Equipment)
			{
				return Equipment->IsEquipped();
			}
		}
	}
	return false;
}

// Called when the game starts or when spawned
void ACDoAction::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	// 데이터 가져오는 것을 먼저 해야 함 BP의 호출순서
	Super::BeginPlay();	
}

// Called every frame
void ACDoAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDoAction::Server_PlayAttackAnimMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	MC_PlayAttackAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void ACDoAction::MC_PlayAttackAnimMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	// AttachmentStatus 의 변수들 불러오기
	float atkSpeed = 1.0f;
	atkSpeed *= (GetCurrentStatus()->CurrentAtkSpeed * InPlayRate);
	OwnerCharacter->PlayAnimMontage(AnimMontage, atkSpeed, StartSectionName);
}

void ACDoAction::SendDamage(float Damage, FDamageEvent& E, ACharacter* InAttacker,  AActor* InCauser, ACharacter* InOtherCharacter)
{
	// 무기스탯 계산 전
	float finalPower = 1.0f;
	finalPower *= (GetCurrentStatus()->CurrentPower * Damage);
	InOtherCharacter->TakeDamage(finalPower, E, InAttacker->GetController(), InCauser);
}

UCAttachmentStatusComponent* ACDoAction::GetCurrentStatus()
{
	// 널 검사는 하지말자. 안될리가 없어
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	UCActionObjectContainer* container = actionComp->GetCurrent();
	return container->GetAttachment()->GetAttachmentStatusComponent();
}

