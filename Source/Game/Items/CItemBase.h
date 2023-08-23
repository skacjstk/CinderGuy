// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IInteract.h"
#include "Components/CItemDataComponent.h"
#include "CItemBase.generated.h"

UCLASS()
class GAME_API ACItemBase : public AActor , public IIInteract
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
	// IInteract 상속
	virtual bool InteractWith_Implementation(class ACharacter* playerCharacter) override;

	class UStaticMeshComponent* GetMesh() { return ItemMesh; }
	class UCItemDataComponent* GetItemData() { return CItemData; }
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* ItemMesh;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UCItemDataComponent* CItemData;
};
