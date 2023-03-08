// Fill out your copyright notice in the Description page of Project Settings.


#include "CStateComponent.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
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
void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}
void UCStateComponent::ChangeType(EStateType InNewType)
{
	EStateType prevType = Type;
	Type = InNewType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);
}