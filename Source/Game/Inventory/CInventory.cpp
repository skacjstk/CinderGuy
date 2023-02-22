#include "CInventory.h"
#include "Item/CItemBase.h"

void ACInventory::SetSlotVisibility(bool value)
{
}

void ACInventory::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.SetNum(20, true);
}


void ACInventory::EquipItem(int32 DragIndex, int32 DropIndex, class ACItemBase** prevItem, class ACItemBase** newItem)
{
	if (DropIndex < 0 || DropIndex > InventorySlots.Num())	// 범위 검사
		return;


}