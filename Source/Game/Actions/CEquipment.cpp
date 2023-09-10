#include "CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void ACEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACEquipment, bEquipped);
}

ACEquipment::ACEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACEquipment::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!!OwnerCharacter) {
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	}	
}

void ACEquipment::Equip_Implementation()
{
	// 장착몽타주 재생
	// 상태 equip

	State->SetEquipMode(true);

	if(!!Data.AnimMontage)	// 몽타주가 있으면, 몽타주에서 노티파이로 호출
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	else 
	{
		Begin_Equip();
		End_Equip();
	}
	// 소유자 캐릭터의 회전 상태 변환하기
	OwnerCharacter->bUseControllerRotationYaw = Data.bPawnControl;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = !Data.bPawnControl;

	IICharacter* characterInterface = Cast<IICharacter>(OwnerCharacter);
	CheckNull(characterInterface);
	characterInterface->ChangeColor(Color);	// ActionData 에서 생성할때 저장됨

}

void ACEquipment::Begin_Equip_Implementation()
{	
	// 델리게이트 콜 (소켓 Attach)
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode(true);
	bEquipped = true;
}

void ACEquipment::Unequip_Implementation()
{
	// 소유자 캐릭터의 회전 상태 변환하기
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();

	bEquipped = false;
}