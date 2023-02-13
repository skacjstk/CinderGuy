#include "CDoAction_Throw.h"
#include "Global.h"
#include "CAim.h"
#include "CThrow.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"


void ACDoAction_Throw::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();	// 언리얼의 new 
	Aim->BeginPlay(OwnerCharacter);

	// 액션 컴포넌트를 가져와서, 
	UCActionComponent* actionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	CheckNull(actionComp);
	actionComp->OnActionTypeChanged.AddDynamic(this, &ACDoAction_Throw::AbortByActionTypeChanged);
}
void ACDoAction_Throw::DoAction()
{
	Super::DoAction();
//	CheckFalse(Aim->IsAvaliable());	// Ai는 Aim이 없으니까 공통점을 위해 사용 안함
	CheckFalse(State->IsIdleMode());

	if (Aim->IsAvaliable())
	{
		if (Aim->IsZooming() == false)
		{
			Aim->On();
		}
	}

	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	CheckNull(Datas[0].ThrowClass);	// 널 체크 
	
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Throw");	// 소켓 찾기
	FRotator rotation = OwnerCharacter->GetController()->GetControlRotation();

	FTransform transform = Datas[0].EffectTransform;
	transform.AddToTranslation(location);
	transform.SetRotation(FQuat(rotation));
	// 투사체 생성
	ThrowObject = GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, OwnerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	ThrowObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);	// 오버랩 이벤트 바인딩( 투사체의 BeginOverlap을 가져오는 셈 )
	UGameplayStatics::FinishSpawningActor(ThrowObject, transform);
}

void ACDoAction_Throw::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Throw::OnAim()
{
	Aim->On();
}

void ACDoAction_Throw::OffAim()
{
	Aim->Off();
}

void ACDoAction_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
	Aim->Tick(DeltaTime);
}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), ThrowObject);

}

void ACDoAction_Throw::AbortByActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	CheckFalse(Aim->IsAvaliable());
	CheckFalse(Aim->IsZooming());
	Aim->Off();
}
