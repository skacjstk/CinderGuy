
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
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interact")
		FText LookAt(AActor* InActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		bool InteractWith(class ACharacter* playerCharacter);
	virtual bool InteractWith_Implementation(class ACharacter* playerCharacter);
};
