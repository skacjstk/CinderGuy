#pragma once

#include "CoreMinimal.h"
#include "Item/CItemBase.h"
#include "Components/CStateComponent.h"
#include "CRuneBase.generated.h"

UCLASS()
class GAME_API ACRuneBase : public ACItemBase
{
	GENERATED_BODY()


public:
	virtual void OnCheckRuneActiveState(EStateType InPrevType, EStateType InNewType);
public:
	virtual	void LoadItem(ACInventoryBase* NewInventory)	override;	// 인벤토리에 적재시 수행해야 하는 함수
	virtual void UnloadItem(ACInventoryBase* NewInventory) override;	// 인벤토리에 하역시 수행해야 하는 함수

private:
	float PowerAlpha = 0.0f;
	float SpeedAlpha = 0.0f;

};
