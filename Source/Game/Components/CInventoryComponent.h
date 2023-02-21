#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInventoryComponent();
	
	void InitInvenSize(uint32 size) { InvenSize = size; }
	void Open();
	void Close();
	
protected:
	virtual void BeginPlay() override;

public:
	bool bActive = false;
private:
	UPROPERTY(EditDefaultsOnly)
		uint32 InvenSize = 20;

	class AActor* OwnerActor;
	class ACInventoryBase* Inventory;

	// 인벤토리 TSubClassOf
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TSubclassOf<class ACRuneInventory>	RuneInventoryClass;
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TSubclassOf<class ACInventory>	InventoryClass;


	// 인벤토리 UI
};
