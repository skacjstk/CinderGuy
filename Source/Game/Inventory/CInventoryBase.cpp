#include "CInventoryBase.h"
#include "Item/CItemBase.h"
#include "Widgets/CUserWidget_Inventory.h"
#include "Characters/CPlayer.h"
#include "Global.h"

ACInventoryBase::ACInventoryBase()
{
	CHelpers::GetClass<UCUserWidget_Inventory>(&InventoryUIClass, TEXT("/Game/Widgets/Inventory/WB_Inventory"));
}

void ACInventoryBase::SwapItem(ACItemBase** itemA, ACItemBase** itemB)
{
	ACItemBase* temp;
	temp = *itemA;
	itemA = itemB;
	*itemB = temp;
}

void ACInventoryBase::OpenInven()
{
	if (InventoryUI == nullptr)
	{
		CLog::Print("Create Inventory Failed");
		return;
	}
	InventoryUI->SetVisibility(ESlateVisibility::Visible);
	SetSlotVisibility(true);

}

void ACInventoryBase::CloseInven()
{
	if (InventoryUI == nullptr)
	{
		CLog::Print("Create Inventory Failed");
		return;
	}
	InventoryUI->SetVisibility(ESlateVisibility::Collapsed);
	SetSlotVisibility(false);
}

void ACInventoryBase::BeginPlay()
{
	Super::BeginPlay();
	ACPlayer* player = Cast<ACPlayer>(GetOwner());
	if (player != nullptr) {
		InventoryUI = CreateWidget<UCUserWidget_Inventory, APlayerController>(Cast<APlayerController>(player->GetController()), InventoryUIClass);
		InventoryUI->AddToViewport();
		InventoryUI->SetVisibility(ESlateVisibility::Hidden);
		CLog::Print("Owner is Player");
	}
	else if(Cast<ACPlayer>(GetOwner()->GetOwner()))
	{
		// Todo: Attachment  이기 때문에 Rune 전용 Widget을 만들어 주어야 한다.
		InventoryUI = CreateWidget<UCUserWidget_Inventory, APlayerController>(Cast<APlayerController>(player->GetController()), InventoryUIClass);
		InventoryUI->AddToViewport();
		InventoryUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

