// Fill out your copyright notice in the Description page of Project Settings.


#include "CStateComponent.h"
#include "Utilities/CLog.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UCStateComponent::SetIdleMode()
{
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

void UCStateComponent::SetActionMode()
{
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

void UCStateComponent::SetStrongActionMode()
{
	ChangeType(EStateType::StrongAction);
}

void UCStateComponent::SetEndingStrongActionMode()
{
	ChangeType(EStateType::EndingStrongAction);
}

void UCStateComponent::SetEquipMode()
{
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
void UCStateComponent::Server_ChangeType_Implementation(EStateType InNewType, AActor* DamageCauser)
{
	MC_ChangeType(InNewType, DamageCauser);
}

void UCStateComponent::ChangeType(EStateType InNewType, AActor* DamageCauser)
{
	MC_ChangeType(InNewType, DamageCauser);	// TODO: 테스트 후 삭제
}

void UCStateComponent::MC_ChangeType_Implementation(EStateType InNewType, AActor* DamageCauser)
{
	EStateType prevType = Type;
	Type = InNewType;
	if (OnStateTypePreChanged.IsBound())
		OnStateTypePreChanged.Broadcast(prevType, Type, DamageCauser);

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type, DamageCauser);
}
