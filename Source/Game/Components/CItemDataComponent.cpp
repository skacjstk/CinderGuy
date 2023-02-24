#include "CItemDataComponent.h"
#include "Global.h" 
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"

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

bool UCItemDataComponent::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	CLog::Print("CitemDataComponent: LookAt");
	CLog::Print(InActor->GetName());
	CLog::Print(OutMessage.ToString());
	return true;
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

