// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInventoryBase.generated.h"

UCLASS()
class GAME_API ACInventoryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACInventoryBase();

	AActor* GetOwnerActor() { return OwnerActor; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	AActor* OwnerActor;	// 이건, Player가 될 수도, Attachment가 될 수도 있음
};
