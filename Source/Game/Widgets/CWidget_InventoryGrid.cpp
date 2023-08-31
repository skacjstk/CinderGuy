#include "CWidget_InventoryGrid.h"
#include "Global.h"
#include "CWidget_InventrorySlot.h"
#include "Components/WrapBox.h"
#include "Components/CInventoryComponent.h"


UCWidget_InventoryGrid::UCWidget_InventoryGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
#if WITH_EDITOR
	CHelpers::GetClass(&SlotClass, TEXT("/Game/Widgets/InventoryUI/WB_InventorySlot.WB_InventorySlot_C"));
#endif
}

bool UCWidget_InventoryGrid::Initialize()
{
	return true;
}
void UCWidget_InventoryGrid::NativePreConstruct()
{
	if(!!InventoryComponent)
	{
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UCWidget_InventoryGrid::UpdatedInventory);
		UpdatedInventory();
	}
}

void UCWidget_InventoryGrid::UpdatedInventory()
{
	int index = 0;
	for (FSlot& slot : InventoryComponent->Content)
	{
		// 작동 안되는건 아닌데, DragDropOperation 생성을 동시에 못해서 못씀
		UCWidget_InventrorySlot* widgetSlot = CreateWidget<UCWidget_InventrorySlot>(this, SlotClass, "ItemSlot");
		widgetSlot->InitItem(
			slot.ItemID,
			slot.Quantity,
			InventoryComponent,
			index
		);
		ItemGrid->AddChildToWrapBox(widgetSlot);
		++index;
	}//end for
}