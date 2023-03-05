#include "CRuneInventoryComponent.h"
#include "Global.h"
#include "Items/ItemRuneBase.h"
#include "Actions/CAttachment.h"
#include "Actions/CAttachment.h"
// 룬 검사 및 Equip 호출
void UCRuneInventoryComponent::TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex)
{
	if (InSourceIndex < 0) return;	
	// 룬 아이템만 장착하게 하기 
	EItemType sourceType = GetItemType(InSourceInventory->Content[InSourceIndex].ItemID);
	if (sourceType == EItemType::Rune)	
		Super::TransferSlots(InSourceIndex, InSourceInventory, InDestinationIndex);	
	
	ACAttachment* ownerAttachment = Cast<ACAttachment>(GetOwner());

	if (!!ownerAttachment)
	{
		// 어차피 인벤토리 빠져나갈 땐 부모 Transfer 라서 쓸모가 없다.
		AItemRuneBase* runeActor = SpawnRune(ownerAttachment, InSourceIndex, InSourceInventory);
		if (!!runeActor)
		{
			runeActor->UnequipRune();
			runeActor->Destroy();	// 바로 부순다. 계산용이니까.
		}
		AItemRuneBase* runeActor2 = SpawnRune(ownerAttachment, InDestinationIndex, this);
		if (!!runeActor2)
		{
			runeActor2->EquipRune();
			runeActor2->Destroy();	// 바로 부순다. 계산용이니까.
		}
	}
}
AItemRuneBase* UCRuneInventoryComponent::SpawnRune(ACAttachment* attachment, int32 InIndex, UCInventoryComponent* InInventory)
{
	AItemRuneBase* runeActor = nullptr;
	// Unequip 적용
	CLog::Print(InInventory->Content[InIndex].ItemID.ToString());
	FRune* sourceRune = RuneTable->FindRow<FRune>(InInventory->Content[InIndex].ItemID, "Source is Not Rune");
	if (!!sourceRune)
	{
		FTransform transform;
		//		transform.SetLocation();	// Attachment->DataObjects->ActionComponent->CPlayer 로 가야 함
		runeActor = GetWorld()->SpawnActorDeferred<AItemRuneBase>(sourceRune->RuneClass, transform, GetOwner());
		runeActor->AttachmentStatus = attachment->GetAttachmentStatusComponent();
		runeActor->SetBonusSpeed(sourceRune->BonusSpeed);
		runeActor->SetBonusPower(sourceRune->BonusPower);	// 스탯 설정
		UGameplayStatics::FinishSpawningActor(runeActor, transform);	// 여기가 진짜위치

	}
	return runeActor;
}