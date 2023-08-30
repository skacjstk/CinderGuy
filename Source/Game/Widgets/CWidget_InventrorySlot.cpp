#include "CWidget_InventrorySlot.h"
#include "Global.h"
#include "Engine/DataTable.h"
#include "Components/CInventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Items/ItemRuneBase.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/PlayerInput.h"
#include "Widgets/CWidget_DragPreview.h"
#include "Input/Reply.h"

UCWidget_InventrorySlot::UCWidget_InventrorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITOR
	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemTable = defaultTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> defaultRuneTable(TEXT("/Game/Inventory/DT_RuneData"));
	if (defaultRuneTable.Succeeded())
		RuneTable = defaultRuneTable.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> defaultDragPreview(TEXT("WidgetBlueprint'/Game/Widgets/InventoryUI/WB_DragPreview.WB_DragPreview_C'"));
	if (defaultDragPreview.Succeeded())
		PreviewClass = defaultDragPreview.Class;
#else
	static ConstructorHelpers::FObjectFinder<UClass> defaultTable(TEXT("/Game/Inventory/DT_ItemData.DT_ItemData_C"));
	if (defaultTable.Object)
		ItemTable = Cast<UDataTable>(defaultTable.Object);

	static ConstructorHelpers::FObjectFinder<UClass> defaultRuneTable(TEXT("/Game/Inventory/DT_RuneData.DT_RuneData_C"));
	if (defaultRuneTable.Object)
		RuneTable = Cast<UDataTable>(defaultRuneTable.Object);

	static ConstructorHelpers::FClassFinder<UClass> defaultDragPreview(TEXT("/Game/Widgets/InventoryUI/WB_DragPreview.WB_DragPreview"));
	if (defaultDragPreview.Succeeded())
		PreviewClass = defaultDragPreview.Class;
#endif
}

bool UCWidget_InventrorySlot::Initialize()
{
	if (Super::Initialize())
	{
		return true;
	}
	return false;
}

FText UCWidget_InventrorySlot::GetDescription()
{
	if (!itemDescription.IsEmpty() || ItemTable == nullptr)
		return itemDescription;
	else
	{
		FItem* item = nullptr;

		if (ItemID.IsNone() == false)
			item = ItemTable->FindRow<FItem>(ItemID, "Find Fail");
		if (!!item)
		{
			itemDescription = item->Description;	
			FRune* rune = RuneTable->FindRow<FRune>(ItemID, "Rune Find Fail");	// 룬 테이블에서 찾아
			if (!!rune)
			{
				// Todo: FText 에 대한 이해 부족, 임시로 ItemData에 모두 떄려박기
	//			FFormatNamedArguments Args;
	//			Args.Add(TEXT("Desc"), itemDescription);
	//			Args.Add(TEXT("Side"), rune->SideEffectDesc);
	//			Args.Add(TEXT("Power"), rune->BonusPower);
	//			itemDescription = FText::Format(LOCTEXT("aa", "{Desc} {Side} {Power}"), Args);
			}
			return itemDescription;
		}
	}
	return itemDescription;
}

void UCWidget_InventrorySlot::InitItem(FName InItemID, int32 InQuantity, UCInventoryComponent* InInventoryComponent, int32 InIndex)
{
	ItemID = InItemID;
	Quantity = InQuantity;
	InventoryComponent = InInventoryComponent;
	ContentIndex = InIndex;
	PreConstruct(true);
}

//FReply UCWidget_InventrorySlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
////	Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
//	if (ItemID.IsEqual("None") || ItemID.IsEqual(""))
//	{		
//		return FReply::Unhandled();
//	}
//	FName ActionName = "Action";
//	FKey keyEvent;
//	if (InMouseEvent.IsMouseButtonDown(keyEvent = CHelpers::GetKeyName<FInputActionKeyMapping>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ActionName)))
//	{
//		UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, keyEvent);
//		return FReply::Handled();		
//	}
//	return FReply::Unhandled();
//}

void UCWidget_InventrorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (ItemTable == nullptr)
	{
		CLog::Print("Slot DT_Item is Nullptr");
		return;
	}
	else
	{
		FItem* item = nullptr;

		if(ItemID.IsNone() == false)
			item = ItemTable->FindRow<FItem>(ItemID, "Find Fail");

		if (item != nullptr)
		{
			itemDescription = item->Description;
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

FKey UCWidget_InventrorySlot::GetActionEventKey(FName ActionName)
{
	return CHelpers::GetKeyName<FInputActionKeyMapping>
		(UGameplayStatics::GetPlayerController(GetWorld(), 0), ActionName);
}
