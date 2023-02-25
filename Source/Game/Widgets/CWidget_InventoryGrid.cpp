#include "CWidget_InventoryGrid.h"
#include "Global.h"
#include "CWidget_InventrorySlot.h"
#include "Components/WrapBox.h"
#include "Components/CInventoryComponent.h"


UCWidget_InventoryGrid::UCWidget_InventoryGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CHelpers::GetClass(&SlotClass, TEXT("/Game/Widgets/InventoryUI/WB_InventorySlot"));  
}

bool UCWidget_InventoryGrid::Initialize()
{
	return true;
}
void UCWidget_InventoryGrid::NativePreConstruct()
{
	if(!!InventoryComponent)
	{
		for (FSlot& slot : InventoryComponent->Content)
		{
			// 스폰시 노출 해도 CreateWidget은 그게 안되는 것 같음.
			UCWidget_InventrorySlot* widgetSlot = CreateWidget<UCWidget_InventrorySlot>(this, SlotClass, "ItemSlot");
			widgetSlot->InitItem(
				slot.ItemID,
				slot.Quantity,
				InventoryComponent
			);
			ItemGrid->AddChildToWrapBox(widgetSlot);
		}
	}
}
