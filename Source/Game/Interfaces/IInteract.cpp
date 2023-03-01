#include "IInteract.h"
#include "GameFramework/Character.h"

FText IIInteract::LookAt_Implementation(AActor* InActor)
{
	return FText();
}

bool IIInteract::InteractWith_Implementation(class ACharacter* playerCharacter)
{
	return false;
}
