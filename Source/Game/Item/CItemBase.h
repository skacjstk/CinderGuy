// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory/CInventoryBase.h"
#include "Components/CStateComponent.h"	// EStateType
#include "CItemBase.generated.h"

UCLASS()
class GAME_API ACItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACItemBase();

public:
	virtual	void LoadItem(ACInventoryBase* NewInventory);	// 인벤토리에 적재시 수행해야 하는 함수
	virtual void UnloadItem(ACInventoryBase* NewInventory);	// 인벤토리에 하역시 수행해야 하는 함수

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	ACInventoryBase* OwnerInventory;

private:
	TSubclassOf<class UCUserWidget_ItemSlot> SlotUIClass;
	class UCUserWidget_ItemSlot* SlotUI;

};
