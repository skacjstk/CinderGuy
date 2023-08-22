// Fill out your copyright notice in the Description page of Project Settings.


#include "CItemDropComponent.h"
#include "Utilities/CLog.h"

// Sets default values for this component's properties
UCItemDropComponent::UCItemDropComponent()
{
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemDropCSV"));
	if (defaultTable.Succeeded())
		ItemDataTable = defaultTable.Object;
}


// Called when the game starts
void UCItemDropComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCItemDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCItemDropComponent::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	if (ItemDataTable == nullptr)
		return;

	TArray<FItemDrop_CSV*> AllRows;
	FString ContextString = "NO Item Found";
	ItemDataTable->GetAllRows(ContextString , AllRows);	// ContextString은 에러 메시지

	DropTable.DropItemSet.Empty();
	for (FItemDrop_CSV* row : AllRows)
	{
		if (row->DropTableID == DropTable.DropTableID)
		{
			FDropItem AddingItem;
			AddingItem.ItemCode = row->ItemCode;
			AddingItem.Amount = row->Amount;
			AddingItem.DropChance = row->DropChance;
			DropTable.DropItemSet.Add(AddingItem);
			CLog::Log("Item Found");
		}
	}
}
