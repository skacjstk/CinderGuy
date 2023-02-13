// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "BTTaskNode_Speed.generated.h"

UCLASS()
class GAME_API UBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		EWalkSpeedType Type;
public:
	UBTTaskNode_Speed();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
