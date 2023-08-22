// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structs/CommonStructs.h"
#include "Components/ActorComponent.h"
#include "CItemDropComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCItemDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCItemDropComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
protected:
	UPROPERTY(EditDefaultsOnly)
		FItemDropTable DropTable;
	UPROPERTY(BlueprintReadOnly)
		UDataTable* ItemDataTable;
};
