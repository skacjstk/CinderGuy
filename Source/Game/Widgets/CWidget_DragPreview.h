// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_DragPreview.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UCWidget_DragPreview : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FName ItemID;
};
