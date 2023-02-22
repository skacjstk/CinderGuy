#include "CRuneInventory.h"
#include "Item/CRuneBase.h"

void ACRuneInventory::EquipItem(int32 DragIndex, int32 DropIndex, ACItemBase** prevItem, ACItemBase** newItem)
{
	if (DropIndex < 0 || DropIndex > InventorySlots.Num())	// 범위 검사
		return;

	if (InventorySlots[DropIndex] == nullptr)
	{
		// Todo: 원래 있었던 위치에서 삭제시키기... 를 어디서 할까?	 nullptr에 대한 Swap이 가능하면 좋겠는데
		// Todo: 어쩌면 ItemBase의 매개변수 자체가 더블포인터 이어야 할 지도 모른다.
		
		InventorySlots[DropIndex] = Cast<ACRuneBase>(*newItem);
		(*newItem)->LoadItem(this);
	}
	else
	{
		(*prevItem)->UnloadItem(this);	// 이거 깡통구현 했으면 뭐 상관없고
		(*newItem)->LoadItem(this);
		SwapItem(prevItem, newItem);
	}

}

void ACRuneInventory::SetSlotVisibility(bool value)
{
}

void ACRuneInventory::BeginPlay()
{
	Super::BeginPlay();
	InventorySlots.SetNum(8, true);	
}
