#include "CRuneInventoryComponent.h"
#include "Global.h"
#include "Items/ItemRuneBase.h"
#include "Actions/CAttachment.h"


UCRuneInventoryComponent::UCRuneInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

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
		AItemRuneBase* runeActor = SpawnRune(ownerAttachment, InDestinationIndex, this);
		if (!!runeActor)
		{
			runeActor->EquipRune();
			ownerAttachment->SetAttachmentRune(InDestinationIndex, runeActor);
			// 장착시에는 부수지 말고, Attachment 에 저장해두자 ( 자신의 효과를 저장해두는 셈)
		}
	}
}

void UCRuneInventoryComponent::MC_Update_Blueprint()
{
	ACAttachment* ownerAttachment = Cast<ACAttachment>(GetOwner());
	if (!!ownerAttachment)
	{
		for (int i = 0; i < GetContents().Num(); ++i)
		{
			AItemRuneBase* runeActor = SpawnRune(ownerAttachment, i, this);
			if (!!runeActor)
			{
				runeActor->EquipRune();
				ownerAttachment->SetAttachmentRune(i, runeActor);
			}
		}		
	}
	Super::MC_Update_Blueprint();
}
// 모든 룬 장착해제 (저장 불러올 때 호출)
void UCRuneInventoryComponent::AllRuneUnequip()
{
	ACAttachment* ownerAttachment = Cast<ACAttachment>(GetOwner());
	if (!!ownerAttachment)
	{
		for (AItemRuneBase* runes : ownerAttachment->GetAttachmentRunes())
		{		
			if(!!runes)
			{
				runes->UnequipRune();
				int32 index = ownerAttachment->GetAttachmentRunes().Find(runes);
				if(index >= 0)
					ownerAttachment->RemoveAttachmentRune(index);
			}
		}
	}
}
