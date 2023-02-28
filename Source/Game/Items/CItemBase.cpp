// Fill out your copyright notice in the Description page of Project Settings.


#include "CItemBase.h"
#include "Global.h"

// Sets default values
ACItemBase::ACItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACItemBase::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	return false;
}

bool ACItemBase::InteractWith_Implementation(ACharacter* playerCharacter)
{
	return false;
}

