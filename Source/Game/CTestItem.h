// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteract.h"
#include "CTestItem.generated.h"

UCLASS()
class GAME_API ACTestItem : public AActor , public IIInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTestItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// IInteract 상속
	virtual bool LookAt_Implementation(AActor* InActor, FText& OutMessage) override;
	virtual bool InteractWith_Implementation(class ACharacter* playerCharacter) override;

};
