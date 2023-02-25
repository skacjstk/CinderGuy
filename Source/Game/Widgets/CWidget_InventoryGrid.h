// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_InventoryGrid.generated.h"

UCLASS()
class GAME_API UCWidget_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:
	UCWidget_InventoryGrid(const FObjectInitializer& ObjectInitializer);
	virtual bool Initialize() override;

protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCInventoryComponent* InventoryComponent;
	TSubclassOf<class UUserWidget> SlotClass;
	UPROPERTY(meta = (BindWidget))
		class UWrapBox* ItemGrid;
};
