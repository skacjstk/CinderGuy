#include "CItemDataComponent.h"
#include "Global.h" 
#include "GameFramework/Character.h"
#include "Characters/CPlayer.h"
#include "Components/CInventoryComponent.h"
#include "Utilities/CLog.h"

UCItemDataComponent::UCItemDataComponent()
{
#if WITH_EDITOR
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemID.DataTable = defaultTable.Object;
#endif
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UCItemDataComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ItemID.DataTable == nullptr)
		ItemID.DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), this, TEXT("/Game/Inventory/DT_ItemData")) );
}

bool UCItemDataComponent::InteractWith_Implementation(class ACharacter* InPlayerCharacter)
{
	UCInventoryComponent* inventory = CHelpers::GetComponent<UCInventoryComponent>(InPlayerCharacter);
	if (!!inventory)
	{
		int32 remainQuantity;
		if (inventory->AddToInventory(ItemID.RowName, Quantity, remainQuantity))
		{
			AfterInteract();
			return true;

		}
		CLog::Log("AddToInventoryFail");
	}
	return false;
}

void UCItemDataComponent::AfterInteract_Implementation()
{
	GetOwner()->Destroy();
}