#include "CWidget_InventrorySlot.h"
#include "Global.h"
#include "Engine/DataTable.h"
#include "Components/CInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Widgets/CWidget_DragPreview.h"
#include "Input/Reply.h"

UCWidget_InventrorySlot::UCWidget_InventrorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemTable = defaultTable.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> defaultDragPreview(TEXT("WidgetBlueprint'/Game/Widgets/InventoryUI/WB_DragPreview.WB_DragPreview_C'"));
	if (defaultDragPreview.Succeeded())
		PreviewClass = defaultDragPreview.Class;

}

bool UCWidget_InventrorySlot::Initialize()
{
//	ItemButton->OnClicked.AddDynamic(this, &UCWidget_InventrorySlot::OnItemClicked);	// DragDrop은 이렇게 반환하는게 아님
	return true;
}

void UCWidget_InventrorySlot::InitItem(FName InItemID, int32 InQuantity, UCInventoryComponent* InInventoryComponent, int32 InIndex)
{
	ItemID = InItemID;
	Quantity = InQuantity;
	InventoryComponent = InInventoryComponent;
	ContentIndex = InIndex;
	PreConstruct(true);
}

FReply UCWidget_InventrorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemID.IsEqual("None") || ItemID.IsEqual(""))
	{
		return FReply::Unhandled();
	}
	FName ActionName = "Action";
	FKey keyEvent;
	if (InMouseEvent.IsMouseButtonDown(keyEvent = CHelpers::GetKeyName<FInputActionKeyMapping>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ActionName)))
	{
		UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, keyEvent);

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

//void UCWidget_InventrorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
//{
//	UCWidget_DragPreview* dragPreview = CreateWidget<UCWidget_DragPreview>(GetWorld(), PreviewClass, "DragPreview");
//	dragPreview->ItemID = ItemID;
//	UWidgetBlueprintLibrary::CreateDragDropOperation();
//
//}

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

		if (!ItemImage)
		{
			CLog::Print("No");
		}
		if (!BoxQuantity)
		{

			CLog::Print("No");
		}
		if (!TextQuantity)
		{

			CLog::Print("No");
		}
		if (!ItemButton)
		{
			CLog::Print("No");

		}

		return;


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
