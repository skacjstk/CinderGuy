// Fill out your copyright notice in the Description page of Project Settings.


#include "CStateComponent.h"
#include "Utilities/CLog.h"
#include "Net/UnrealNetwork.h"
#include "Utilities/CHelpers.h"

void UCStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCStateComponent, Type);
	DOREPLIFETIME(UCStateComponent, IFGuardAlpha);
	DOREPLIFETIME(UCStateComponent, IFEvadeAlpha);
}

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCStateComponent::SetIdleMode(bool FromServer)
{
	if(FromServer)
		MC_ChangeStateType(EStateType::Idle);
	else
		ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackStepMode()
{
	ChangeType(EStateType::BackStep);
}

void UCStateComponent::SetActionMode(bool FromServer)
{
	if (FromServer)
		MC_ChangeStateType(EStateType::Action);
	else
		ChangeType(EStateType::Action);
}

void UCStateComponent::SetGuardMode()
{
	ChangeType(EStateType::Guard);
}

void UCStateComponent::SetBlockMode()
{
	ChangeType(EStateType::GuardBack);
}

void UCStateComponent::SetParryMode()
{
	ChangeType(EStateType::Parry);
}

void UCStateComponent::SetStrongActionMode(bool FromServer)
{
	ChangeType(EStateType::StrongAction);
	if (FromServer)
		MC_ChangeStateType(EStateType::StrongAction);
	else
		ChangeType(EStateType::StrongAction);
}

void UCStateComponent::SetEndingStrongActionMode()
{
	ChangeType(EStateType::EndingStrongAction);
}

void UCStateComponent::SetEquipMode(bool FromServer)
{
	if (FromServer)
		MC_ChangeStateType(EStateType::Equip);
	else
		ChangeType(EStateType::Equip);
}
void UCStateComponent::SetHittedMode(AActor* DamageCauser)
{
	ChangeType(EStateType::Hitted, DamageCauser);
}

void UCStateComponent::SetDeadMode(AActor* DamageCauser)
{
	ChangeType(EStateType::Dead, DamageCauser);
}

void UCStateComponent::ChangeType(EStateType InNewType, AActor* DamageCauser)
{
	Server_ChangeStateType(InNewType, DamageCauser);
}

void UCStateComponent::Server_ChangeStateType_Implementation(EStateType InNewType, AActor* DamageCauser)
{
	MC_ChangeStateType(InNewType, DamageCauser);
}

void UCStateComponent::MC_ChangeStateType_Implementation(EStateType InNewType, AActor* DamageCauser)
{
	EStateType prevType = Type;
	Type = InNewType;
	if (OnStateTypePreChanged.IsBound())
		OnStateTypePreChanged.Broadcast(prevType, Type, DamageCauser);

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type, DamageCauser);
}

void UCStateComponent::OnRep_ChangeType()
{
//	CLog::Print("EnteranceTest: ChangeStateType:: " + CHelpers::GetRoleText(GetOwnerRole()) + FString::FromInt((int32)Type));
}
