#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Inventory.generated.h"

UCLASS()
class GAME_API UCUserWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetOwnerInvenComponent(class UCInventoryComponent* InInvenComp);
public:
	UPROPERTY(BlueprintReadWrite)
		class UCInventoryComponent* OwnerInvenComponent;
};
