#include "CDoAction.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "Actions/CAttachment.h"
#include "Components/CAttachmentStatusComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"

// Sets default values
ACDoAction::ACDoAction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ACDoAction::PlayAttackAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	// AttachmentStatus 의 변수들 불러오기

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		float atkSpeed = 1.0f;

		UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(player);
		UCActionObjectContainer* container = actionComp->GetCurrent();
		atkSpeed = container->GetAttachment()->GetAttachmentStatusComponent()->CurrentAtkSpeed;

		OwnerCharacter->PlayAnimMontage(AnimMontage, InPlayRate * atkSpeed, StartSectionName);
	}
	else  // 없으면 일단 공속미적용 재생
	{
		OwnerCharacter->PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);	
	}
}

