// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class GAME_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAIController();

	virtual void Tick(float DeltaTime) override;	// drawDebugCircle로 그려버리려고 
public:	// Get 함수
	float GetSightRadius();	// Perception 의 시야거리 반환
	FORCEINLINE float GetBehaviorRange() { return BehaviorRange; }
protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;	// 빙의 될 때 호출: BeginPlay 보다 먼저 호출됨
	virtual void OnUnPossess() override;	// 빙의 나갈때 호출

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCBehaviorComponent* Behavior;

	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;
	UPROPERTY(EditAnywhere)
		float BehaviorRange = 150.f;
	UPROPERTY(EditAnywhere)
		bool bDrawDebug = true;
	UPROPERTY(EditAnywhere)
		float	InitHeight = 50.f;

private:
	class ACEnemy_AI* OwnerEnemy;
	class UAISenseConfig_Sight* Sight; 
};
