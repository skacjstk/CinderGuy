// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "CDamageText.generated.h"

UCLASS()
class GAME_API ACDamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDamageText();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void FloatingAlpha(float output);

	UFUNCTION()
		void DestroyThis();
public:	
	virtual void Tick(float DeltaTime) override;
	void SetDamage(float damage);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UTextRenderComponent* DamageText;

	class UCurveFloat* Curve;
	FOnTimelineFloat TimelineFloat;
	FTimeline Timeline;
	FOnTimelineEvent EndEvent;

	FVector StartLocation;

};
