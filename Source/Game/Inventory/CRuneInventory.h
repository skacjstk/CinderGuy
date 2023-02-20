#pragma once

#include "CoreMinimal.h"
#include "Inventory/CInventoryBase.h"
#include <array>
#include "CRuneInventory.generated.h"

UCLASS()
class GAME_API ACRuneInventory : public ACInventoryBase
{
	GENERATED_BODY()
	
public:
	virtual void EquipItem(int32 selectedIndex, class ACItemBase** prevItem, class ACItemBase** newItem) override;

	std::array<class ACRuneBase*, 8> GetSlots() { return InventorySlots; }
private:
	std::array<class ACRuneBase*, 8> InventorySlots;	// RuneInventory는 고정 길이 배열이어야 한다.
};
