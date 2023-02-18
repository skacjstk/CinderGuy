// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actions/CActionObjectContainer.h"		// 구조체 가져와야 해서 헤더선언
#include "CAttachmentStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCAttachmentStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCAttachmentStatusComponent();

	void InitStatus(FAttachmentStatusData AttachmentStatusData);

	void IncreasePower(float Percent);	// 1이면 100% 증가임
	void DecreasePower(float Percent);	// 1이면 100% 감소임

	void IncreaseAtkSpeed(float Percent);	// 1이면 100% 증가임
	void DecreaseAtkSpeed(float Percent);	// 1이면 100% 감소임

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	float CurrentPower = 1.0f;
	float CurrentAtkSpeed = 1.0f;	// 현재 값
	
private:
	float BasePower = 1.0f;		// 계산용 변수
	float BaseAtkSpeed = 1.0f;

//	class ? ? ModSlot[8]
		
};
