#include "IInteract.h"
#include "GameFramework/Character.h"

bool IIInteract::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	return false;
}

bool IIInteract::InteractWith_Implementation(class ACharacter* playerCharacter)
{
	return false;
}
