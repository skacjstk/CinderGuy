// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Melee.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UBTService_Melee : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Melee();	 //서비스를 트리에 그냥 붙이지 말고, 
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
