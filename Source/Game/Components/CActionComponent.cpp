#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CEquipment.h"
#include "Actions/CActionObjectContainer.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "Actions/CDoAction.h"
#include "Utilities/CLog.h"
#include "GameFramework/Character.h"
#include "Utilities/CHelpers.h"
#include "Net/UnrealNetwork.h"

void UCActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCActionComponent, Type);
}

UCActionComponent::UCActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* charcater = Cast<ACharacter>(GetOwner());
	for (int i = 0; i < (int32)EActionType::Max; ++i)
	{
		if (!!Datas[i])	// 바뀌면 안됨. 생성부분
		{
			Datas[i]->BeginPlay(charcater, &DataObjects[i], i);	// 만든 결과를 DataObjects에 저장
		}
	}
	FString Result;

	Result += GetOwner()->GetName();
	Result += " | ";
	Result += CHelpers::GetRoleText(GetOwner()->GetLocalRole());

	for (UCActionObjectContainer* DO : DataObjects)
	{
		if (DO != nullptr) {
			FString Result2 = " | ";
			Result2 += DO->GetName();
			CLog::Log(Result + Result2);
		}
	}
}

void UCActionComponent::SetUnarmedMode()
{
	if(!!DataObjects[(int32)Type] && DataObjects[(int32)Type]->GetEquipment())
		DataObjects[(int32)Type]->GetEquipment()->Unequip();

	DataObjects[(int32)EActionType::Unarmed]->GetEquipment()->Equip();

	Server_ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	Server_SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	Server_SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	Server_SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetWarpMode()
{
	Server_SetMode(EActionType::Warp);
}

void UCActionComponent::SetKatanaMode()
{
	Server_SetMode(EActionType::Katana);
}

void UCActionComponent::SetMagicBallMode()
{
	Server_SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetStormMode()
{
	Server_SetMode(EActionType::Storm);
}

void UCActionComponent::Server_SetMode_Implementation(EActionType InNewType)
{
	MC_SetMode(InNewType);
}

void UCActionComponent::MC_SetMode_Implementation(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnarmedMode();	// Unarmed는 전용이 따로 있음 
		return;
	}
	else if (IsUnarmedMode() == false)	// 다른 무기 누르면 해제와 장착을 
	{
		if (!!DataObjects[(int32)Type] && DataObjects[(int32)Type]->GetEquipment())
			DataObjects[(int32)Type]->GetEquipment()->Unequip();
	}

	if (!!DataObjects[(int32)InNewType] && DataObjects[(int32)InNewType]->GetEquipment())
		DataObjects[(int32)InNewType]->GetEquipment()->Equip();

	Server_ChangeType(InNewType);
}

void UCActionComponent::Server_DoAction_Implementation()
{
	MC_DoAction();
}
void UCActionComponent::MC_DoAction_Implementation()
{
	CheckTrue(IsUnarmedMode());

	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->DoAction();
		else
			CLog::Log("What");
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

void UCActionComponent::EndDoStrongActionWait()
{
	CheckTrue(IsUnarmedMode());
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->EndDoStrongActionWait();	// 강공격 해제 대기
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

void UCActionComponent::DoParry()
{
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OnParry();
	}
}

void UCActionComponent::DoBlock()
{
	if (!!DataObjects[(int32)Type])
	{
		ACDoAction* doAction = DataObjects[(int32)Type]->GetDoAction();

		if (!!doAction)
			doAction->OnBlock();
	}
}

void UCActionComponent::Server_ChangeType_Implementation(EActionType InNewType)
{
	MC_ChangeType(InNewType);
}

void UCActionComponent::MC_ChangeType_Implementation(EActionType InNewType)
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
	// ActionComponent가 가진 것들 지우기
	for (int32 i = 0; i < (int32)EActionType::Max; ++i)
	{
		if (!!DataObjects[i] && !!DataObjects[i]->GetAttachment())
			DataObjects[i]->GetAttachment()->Destroy();

		if (!!DataObjects[i] && !!DataObjects[i]->GetEquipment())
			DataObjects[i]->GetEquipment()->Destroy();

		if (!!DataObjects[i] && !!DataObjects[i]->GetDoAction())
			DataObjects[i]->GetDoAction()->Destroy();
	}
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

void UCActionComponent::AbortByDamaged()
{
	CheckNull(DataObjects[(int32)Type]);
	CheckTrue(IsUnarmedMode());

	DataObjects[(int32)Type]->GetEquipment()->Begin_Equip();
	DataObjects[(int32)Type]->GetEquipment()->End_Equip();	// Idle로 돌려주는 코드

	DataObjects[(int32)Type]->GetDoAction()->Abort();
}