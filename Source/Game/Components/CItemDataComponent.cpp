#include "CItemDataComponent.h"
#include "Global.h"

UCItemDataComponent::UCItemDataComponent()
{
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("DataTable'/Game/Inventory/DT_Item.DT_Item'"));
	if (defaultTable.Succeeded())
		ItemID.DataTable = defaultTable.Object;
}


// Called when the game starts
void UCItemDataComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->SetReplicates(true);	
}

bool UCItemDataComponent::InteractWith_Implementation()
{
	GetOwner()->Destroy();
	return true;
}

