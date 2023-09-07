// Fill out your copyright notice in the Description page of Project Settings.


#include "CReplicaBehaviorComponent.h"

// Sets default values for this component's properties
UCReplicaBehaviorComponent::UCReplicaBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCReplicaBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCReplicaBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

