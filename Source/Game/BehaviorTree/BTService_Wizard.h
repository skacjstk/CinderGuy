// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Wizard.generated.h"

UCLASS()
class GAME_API UBTService_Wizard : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Wizard();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
