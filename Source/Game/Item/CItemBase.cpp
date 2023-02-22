#include "CItemBase.h"
#include "Widgets/CUserWidget_ItemSlot.h"
#include "Global.h"

ACItemBase::ACItemBase()
{
	CHelpers::GetClass<UCUserWidget_ItemSlot>(&SlotUIClass, "/Game/Widgets/Inventory/WB_Inventory_Slot");

	SlotUI = CreateWidget<UCUserWidget_ItemSlot>(GetWorld(), SlotUIClass);
	SlotUI->AddToViewport();
	
}

void ACItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACItemBase::LoadItem(ACInventoryBase* NewInventory)
{
	
}

void ACItemBase::UnloadItem(ACInventoryBase* NewInventory)
{
	// 기존 소속은 OWnerInventory로 되어있고, newInventory가 Rune이 아니면 Rune의 그거 호출
}
