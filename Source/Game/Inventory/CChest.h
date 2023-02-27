// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteract.h"
#include "CChest.generated.h"

UCLASS()
class GAME_API ACChest : public AActor , public IIInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;
};
