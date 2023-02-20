#include "CInventoryBase.h"
#include "Item/CItemBase.h"

ACInventoryBase::ACInventoryBase()
{

}

void ACInventoryBase::SwapItem(ACItemBase** itemA, ACItemBase** itemB)
{
	ACItemBase* temp;
	temp = *itemA;
	itemA = itemB;
	*itemB = temp;

	// 과거 D2D에서 했던 Swap 코드
//	Item* temp = (*dragSlot)->GetItem();
//	(*dragSlot)->SetItem((*dropSlot)->GetItem());
//	(*dropSlot)->SetItem(temp);
}

void ACInventoryBase::BeginPlay()
{
	Super::BeginPlay();
	
}

