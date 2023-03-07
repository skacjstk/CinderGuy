#pragma once

#include "CoreMinimal.h"
#include "Components/CInventoryComponent.h"
#include "CRuneInventoryComponent.generated.h"

UCLASS()
class GAME_API UCRuneInventoryComponent : public UCInventoryComponent
{
	GENERATED_BODY()
public:
	virtual void TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex) override;
};
