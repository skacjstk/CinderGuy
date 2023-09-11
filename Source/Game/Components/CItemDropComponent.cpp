// Fill out your copyright notice in the Description page of Project Settings.


#include "CItemDropComponent.h"
#include "Items/CItemBase.h"
#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"
#include "Utilities/CLog.h"

// Sets default values for this component's properties
UCItemDropComponent::UCItemDropComponent()
{
#if WITH_EDITOR
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemDropCSV"));
	if (defaultTable.Succeeded())
		ItemDataTable = defaultTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> defaultItemTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemTable = defaultItemTable.Object;
#endif
}


// Called when the game starts
void UCItemDropComponent::BeginPlay()
{
	Super::BeginPlay();
	if(ItemTable == nullptr)
		ItemTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), this, TEXT("/Game/Inventory/DT_ItemData")));
	if(ItemDataTable == nullptr)
		ItemDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), this, TEXT("/Game/Inventory/DT_ItemDropCSV")));
	
}


// Called every frame
void UCItemDropComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#if WITH_EDITOR
void UCItemDropComponent::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	SettingValue();
}
#endif

void UCItemDropComponent::SettingValue()
{
	if (ItemDataTable == nullptr)
		return;

	TArray<FItemDrop_CSV*> AllRows;

	DropTable.DropItemSet.Empty();
	ItemDataTable->GetAllRows("NO Item Found", AllRows);
	if (AllRows.Num() <= 0)
		return;

	for (FItemDrop_CSV* row : AllRows)
	{
		if (row->DropTableID == DropTable.DropTableID)
		{
			FDropItem AddingItem;
			AddingItem.ItemCode = row->ItemCode;
			AddingItem.Amount = row->Amount;
			AddingItem.DropChance = row->DropChance;
			DropTable.DropItemSet.Add(AddingItem);
		}
	}
}
void UCItemDropComponent::DropItem(FVector DropLocation)
{
	SettingValue();

	for (FDropItem& item : DropTable.DropItemSet)
	{
		FString IntFromString = FString::FromInt(item.ItemCode);
		
		FName ItemName = FName(IntFromString);

		if (ItemName.IsNone())
			continue;
		FItem* SearchItem = ItemTable->FindRow<FItem>(ItemName, "Find Fail");

		if (SearchItem != nullptr)
		{
			float Dice = FMath::RandRange(0.0f, 1.0f);

			if (item.DropChance > Dice)
			{
				ACItemBase* itemActor = GetWorld()->SpawnActor<ACItemBase>(SearchItem->ItemClass);
				if (itemActor != nullptr)
				{
					float Direction1 = FMath::RandRange(-1.f, 1.f);
					float Direction2 = FMath::RandRange(-1.f, 1.f);
					itemActor->SetActorLocation(DropLocation);
					itemActor->GetItemData()->Quantity = item.Amount;
					FVector Force = FVector(Direction1 * 10000000.0f, Direction2 * 10000000.0f, 1000000.0f);
						//FVector(Dice - 0.5f * 100000.f, Dice - 0.5f * 100.f, Dice * 100.0f);
					itemActor->GetMesh()->AddForce(Force);
				}
			}
		}
	}
}
