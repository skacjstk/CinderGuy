#pragma once

#include "CoreMinimal.h"
#include "Inventory/CInventoryBase.h"
#include "CInventory.generated.h"
/*
* Character's Inventroy ex) CPlayer, CEnemy, etc.... 
*/
UCLASS()
class GAME_API ACInventory : public ACInventoryBase
{
	GENERATED_BODY()
public:
	virtual void EquipItem(int32 DragIndex, int32 DropIndex, class ACItemBase** prevItem, class ACItemBase** newItem) override;
	virtual void GetSlots(TArray<class ACItemBase*> OutSlots) override { OutSlots = InventorySlots; }

	virtual void SetSlotVisibility(bool value);
protected:
	virtual void BeginPlay() override;

private:
	TArray<class ACItemBase*> InventorySlots;	// RuneInventory는 고정 길이 배열이어야 한다.
};
