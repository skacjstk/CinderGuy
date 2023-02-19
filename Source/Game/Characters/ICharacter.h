// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAME_API IICharacter
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ChangeColor(FLinearColor InColor) = 0;
	virtual void Hitted() = 0;
	virtual void Dead() = 0;
	virtual void End_Dead() = 0;
};
