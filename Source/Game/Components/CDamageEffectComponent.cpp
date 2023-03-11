// Fill out your copyright notice in the Description page of Project Settings.


#include "CDamageEffectComponent.h"

// Sets default values for this component's properties
UCDamageEffectComponent::UCDamageEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCDamageEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UCDamageEffectComponent::DamageEffect()
{
	//Todo: 여기서 받아야 할 Enum, 그 Enum 간의 처리를 구현하기
	return false;
}
