#include "CWidget_InventrorySlot.h"
#include "Global.h"
#include "Engine/DataTable.h"
#include "Components/CInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetTree.h"

UCWidget_InventrorySlot::UCWidget_InventrorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemTable = defaultTable.Object;
}

void UCWidget_InventrorySlot::InitItem(FName InItemID, int32 InQuantity, UCInventoryComponent* InInventoryComponent)
{
	ItemID = InItemID;
	Quantity = InQuantity;
	InventoryComponent = InInventoryComponent;
	PreConstruct(false);	// 이거 되나? 
}

void UCWidget_InventrorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	UE_LOG(LogTemp, Warning, TEXT("Slot itemID is %s"), *ItemID.ToString());
	TArray<FName> rowNames;

	rowNames = ItemTable->GetRowNames();
	for (FName& rowName : rowNames)
	{
		UE_LOG(LogTemp, Warning, TEXT("rowName is %s"), *rowName.ToString());
	}
	if (ItemTable == nullptr)
	{
		CLog::Print("Slot DT_Item is Nullptr");
		return;
	}
	else
	{		
		FItem* item = ItemTable->FindRow<FItem>(ItemID, "Find Fail");
		if (item != nullptr)
		{
			ItemImage->SetBrushFromTexture(item->Thumbnail, false);

			if (Quantity <= 0)
			{
				ItemImage->SetVisibility(ESlateVisibility::Hidden);
				BoxQuantity->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				ItemImage->SetVisibility(ESlateVisibility::Visible);
				BoxQuantity->SetVisibility(ESlateVisibility::Visible);
				TextQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
			}
		}
		else // 아이템 못찾으면
		{
			ItemImage->SetVisibility(ESlateVisibility::Hidden);
			BoxQuantity->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UCWidget_InventrorySlot::InitWidgets(UButton* InButton, UImage* InImage, UTextBlock* InText, USizeBox* InBox)
{
	ItemButton = InButton;
	ItemImage = InImage;
	TextQuantity = InText; 
	BoxQuantity = InBox;
}
