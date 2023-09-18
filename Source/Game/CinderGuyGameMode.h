// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CinderGuyGameMode.generated.h"

UCLASS()
class GAME_API ACinderGuyGameMode : public AGameMode
{
	GENERATED_BODY()
public: 
	ACinderGuyGameMode();
	virtual void PostLogin(APlayerController* NewController) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable)
		void Travel(FString LevelName);
};
