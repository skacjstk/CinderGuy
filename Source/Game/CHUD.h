// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class GAME_API ACHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACHUD();
	virtual void DrawHUD() override;

	FORCEINLINE void VisibleAim() { bVisibleAim = true; }
	FORCEINLINE void HiddenAim() { bVisibleAim = false; }

private:
	UPROPERTY(EditAnywhere)
		class UTexture2D* Texture;
	bool bVisibleAim;
};
