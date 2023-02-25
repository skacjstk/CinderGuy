#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_PlayerHUD.generated.h"

UCLASS()
class GAME_API UCWidget_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "HUD")
		void DisplayPlayerMenu();
	virtual void DisplayPlayerMenu_Implementation();
};
