// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CActionComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class GAME_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EStateType StateType;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UFUNCTION()
		void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
};
