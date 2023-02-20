#include "CRuneInventory.h"
#include "Item/CRuneBase.h"
#include <array>

void ACRuneInventory::EquipItem(int32 selectedIndex, ACItemBase** prevItem, ACItemBase** newItem)
{
	if (selectedIndex < 0 || selectedIndex > InventorySlots.size())	// 범위 검사
		return;

	if (InventorySlots[selectedIndex] == nullptr)
	{
		// Todo: 원래 있었던 위치에서 삭제시키기... 를 어디서 할까?	 nullptr에 대한 Swap이 가능하면 좋겠는데
		// Todo: 어쩌면 ItemBase의 매개변수 자체가 더블포인터 이어야 할 지도 모른다.
		
		InventorySlots[selectedIndex] = Cast<ACRuneBase>(*newItem);
		(*newItem)->LoadItem(this);
	}
	else
	{
		(*prevItem)->UnloadItem(this);	// 이거 깡통구현 했으면 뭐 상관없고
		(*newItem)->LoadItem(this);
		SwapItem(prevItem, newItem);
	}

}