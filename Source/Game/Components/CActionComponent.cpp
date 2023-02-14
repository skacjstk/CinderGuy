#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CEquipment.h"
#include "Actions/CActionObjectContainer.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* charcater = Cast<ACharacter>(GetOwner());
	for (int i = 0; i < (int32)EActionType::Max; ++i)
	{
		if(!!Datas[i])	// 바뀌면 안됨. 생성부분
			Datas[i]->BeginPlay(charcater, &DataObjects[i]);	// 만든 결과를 DataObjects에 저장
	}
}

void UCActionComponent::SetUnarmedMode()
{
	if(!!DataObjects[(int32)Type] && DataObjects[(int32)Type]->GetEquipment())
		DataObjects[(int32)Type]->GetEquipment()->Unequip();

	DataObjects[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetStormMode()
{
	SetMode(EActionType::Storm);
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());

	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->DoAction();
	}
}

void UCActionComponent::DoStrongAction()
{
	CheckTrue(IsUnarmedMode());
	// Todo: StrongAction 상태일 경우 CheckTrue()
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->DoStrongAction();
	}
}

void UCActionComponent::DoOnAim()
{
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OnAim();
	}
}

void UCActionComponent::DoOffAim()
{
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OffAim();
	}
}

void UCActionComponent::SetMode(EActionType InNewType)
{	
	if (Type == InNewType)
	{
		SetUnarmedMode();	// Unarmed는 전용이 따로 있음 
		return;
	}
	else if (IsUnarmedMode() == false)	// 다른 무기 누르면 해제와 장착을 
	{
		CLog::Print((int32)InNewType);

		if (!!DataObjects[(int32)Type] && DataObjects[(int32)Type]->GetEquipment())
			DataObjects[(int32)Type]->GetEquipment()->Unequip();
	}
	
	if (!!DataObjects[(int32)InNewType] && DataObjects[(int32)InNewType]->GetEquipment())
		DataObjects[(int32)InNewType]->GetEquipment()->Equip();
	
	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType prevType = Type;
	Type = InNewType;
	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, InNewType);
}

void UCActionComponent::Dead()
{
	OffAllCollisions();
}

void UCActionComponent::End_Dead()
{
	// Tod. All Attachment, Equipment, DoAction Release
}

void UCActionComponent::OffAllCollisions()
{
	for (const auto& data : DataObjects)
	{
		if (data == nullptr || data->GetAttachment() == nullptr)
			continue;
		data->GetAttachment()->OffCollisions();
	}
}
