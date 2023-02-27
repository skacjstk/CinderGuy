#include "CHelpersBlueprint.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"

FKey UCHelpersBlueprint::GetKeyName(class APlayerController* Controller, FName ActionName)
{
	return CHelpers::GetKeyName<FInputActionKeyMapping>(Controller, ActionName);
}
