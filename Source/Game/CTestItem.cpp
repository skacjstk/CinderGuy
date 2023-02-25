// Fill out your copyright notice in the Description page of Project Settings.


#include "CTestItem.h"
#include "Global.h"

// Sets default values
ACTestItem::ACTestItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTestItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTestItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACTestItem::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	CLog::Print("Here is CPP");
	return false;
}

bool ACTestItem::InteractWith_Implementation(ACharacter* playerCharacter)
{
	CLog::Print("Here is CPP");
	return false;
}

