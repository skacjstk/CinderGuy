#include "CRuneBase.h"
#include "Global.h"
#include "Inventory/CRuneInventory.h"
#include "Actions/CAttachment.h"
#include "Components/CStateComponent.h"
#include "Components/CAttachmentStatusComponent.h"

void ACRuneBase::LoadItem(ACInventoryBase* NewInventory)
{
	// 무기에 끼운거면
	if (Cast<ACRuneInventory>(NewInventory))	// 룬 인벤토리 일 경우: 이건 무조건 Attachment 임
	{
		OwnerInventory = NewInventory;
		// Attachment 가져오기
		if (ACAttachment* attachment = Cast<ACAttachment>(OwnerInventory->GetOwnerActor()))
		{
			// 무기 Status 에 Rune 효과 발동하기
			attachment->GetAttachmentStatusComponent()->IncreasePower(PowerAlpha);
			attachment->GetAttachmentStatusComponent()->IncreaseAtkSpeed(SpeedAlpha);

			UCStateComponent* state = nullptr;
			if(!!attachment->GetOwner())
				state = CHelpers::GetComponent<UCStateComponent>(attachment->GetOwner());
			if(state != nullptr)
				state->OnStateTypeChanged.AddDynamic(this, &ACRuneBase::OnCheckRuneActiveState);
		}
	}
	// CharacterInventory 일 경우( 룬 장착같은 개념이 이루어지면 안된다 )
	else
	{
		OwnerInventory = NewInventory;
	}
}

void ACRuneBase::UnloadItem(ACInventoryBase* PrevInventory)
{
	// 무기에서 떼온거면
	if (Cast<ACRuneInventory>(OwnerInventory))	// 룬 인벤토리 일 경우: 이건 무조건 Attachment 임
	{
		// Attachment 가져오기
		if (ACAttachment* attachment = Cast<ACAttachment>(OwnerInventory->GetOwnerActor()))
		{
			// 무기 Status 에 Rune 효과 발동하기
			attachment->GetAttachmentStatusComponent()->DecreasePower(PowerAlpha);
			attachment->GetAttachmentStatusComponent()->DecreaseAtkSpeed(SpeedAlpha);

			UCStateComponent* state = nullptr;
			if (!!attachment->GetOwner())
				state = CHelpers::GetComponent<UCStateComponent>(attachment->GetOwner());
			if (state != nullptr)
				state->OnStateTypeChanged.Remove(this, "OnCheckRuneActiveState");
		}
	}
	// CharacterInventory 일 경우( 룬 장착같은 개념이 이루어지면 안된다 )
	else
	{
		// Unload 에서는 뭐해야하지?
	}
}

void ACRuneBase::OnCheckRuneActiveState(EStateType InPrevType, EStateType InNewType)
{
	CLog::Print("Rune Delegate Activated!!");
}



