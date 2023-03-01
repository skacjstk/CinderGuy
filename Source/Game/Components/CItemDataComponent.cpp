#include "CItemDataComponent.h"
#include "Global.h" 
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"

UCItemDataComponent::UCItemDataComponent()
{
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemID.DataTable = defaultTable.Object;
}


// Called when the game starts
void UCItemDataComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->SetReplicates(true);	
}

FText UCItemDataComponent::LookAt_Implementation(AActor* InActor)
{
	return FText();
}

bool UCItemDataComponent::InteractWith_Implementation(class ACharacter* InPlayerCharacter)
{
	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(InPlayerCharacter);
	if (!!inventory)
	{
		int32 remainQuantity;
		if (inventory->AddToInventory(ItemID.RowName, Quantity, remainQuantity))
		{
			GetOwner()->Destroy();
			return true;
		}
	}
	return false;
}

