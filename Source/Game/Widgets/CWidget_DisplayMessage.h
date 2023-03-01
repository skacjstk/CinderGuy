#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_DisplayMessage.generated.h"

UCLASS()
class GAME_API UCWidget_DisplayMessage : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "DisplayMessage")
		void ShowMessage(const FText& Message, int32 InQuantity);
};
