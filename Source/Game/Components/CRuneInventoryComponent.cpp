#include "CRuneInventoryComponent.h"

// 룬 검사 및 Equip 호출
void UCRuneInventoryComponent::TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex)
{
	if (InSourceIndex < 0) return;
	
	// 룬 아이템만 장착하게 하기 

	EItemType sourceType = GetItemType(InSourceInventory->Content[InSourceIndex].ItemID);
	if (sourceType == EItemType::Rune)
	{
		Super::TransferSlots(InSourceIndex, InSourceInventory, InDestinationIndex);
	}


}