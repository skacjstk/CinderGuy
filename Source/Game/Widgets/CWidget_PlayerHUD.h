#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_PlayerHUD.generated.h"

UCLASS()
class GAME_API UCWidget_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
		void DisplayPlayerMenu();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
		void ShowContainer(UCInventoryComponent* ContainerInventoryComp);
};
