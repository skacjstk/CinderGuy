
#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IInteract.generated.h"

UINTERFACE(Blueprintable, MinimalAPI)
class UIInteract : public UInterface
{
	GENERATED_BODY()
};

class GAME_API IIInteract
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		bool LookAt(AActor* InActor, FText& OutMessage);
	virtual bool LookAt_Implementation(class AActor* InActor, FText& OutMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		bool InteractWith();
	virtual bool InteractWith_Implementation();
};
