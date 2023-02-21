#pragma once

#include "CoreMinimal.h"
#include "Inventory/CInventoryBase.h"
#include "CRuneInventory.generated.h"

UCLASS()
class GAME_API ACRuneInventory : public ACInventoryBase
{
	GENERATED_BODY()
	
public:
	virtual void EquipItem(int32 DragIndex, int32 DropIndex, class ACItemBase** prevItem, class ACItemBase** newItem) override;


	virtual void GetSlots(TArray<class ACRuneBase*> OutSlots) { OutSlots = InventorySlots; }	// 얘는 오버로딩

protected:
	virtual void BeginPlay() override;

private:
	TArray<class ACRuneBase*> InventorySlots;	// RuneInventory는 고정 길이 배열이어야 한다. ...근데 그냥 TArray로 변경
};
