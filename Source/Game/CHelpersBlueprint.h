#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CHelpersBlueprint.generated.h"

UCLASS()
class GAME_API UCHelpersBlueprint : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Key")
		static FKey GetKeyName(class APlayerController* Controller, FName ActionName);
};
