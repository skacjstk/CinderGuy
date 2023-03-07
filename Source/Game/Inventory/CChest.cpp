#include "CChest.h"
#include "Global.h"
#include "Components/CInventoryComponent.h"

ACChest::ACChest()
{
	PrimaryActorTick.bCanEverTick = false;
	CHelpers::CreateActorComponent(this, &InventoryComponent, "Inventory");
	if (!!InventoryComponent)
		InventoryComponent->SetInventorySize(8);
}

// Called when the game starts or when spawned
void ACChest::BeginPlay()
{
	Super::BeginPlay();
}

