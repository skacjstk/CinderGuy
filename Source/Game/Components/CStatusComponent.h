// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"
UENUM(BlueprintType)
enum class EWalkSpeedType : uint8
{
	Sneak, Walk, Run, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();
	UFUNCTION(BlueprintCallable, Category = "Status")
		void IncreaseHealth(float InAmount);
	UFUNCTION(BlueprintCallable, Category = "Status")
		void DecreaseHealth(float InAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetSneakSpeed() {return Speed[(int32)EWalkSpeedType::Sneak];}
	FORCEINLINE float GetWalkSpeed()  {return Speed[(int32)EWalkSpeedType::Walk];}
	FORCEINLINE float GetRunSpeed()   {return Speed[(int32)EWalkSpeedType::Run];}
	FORCEINLINE bool IsCanMove()	{ return bCanMove; }

	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }

	void SetMove();
	void SetStop();

	// Task 접근용 함수
	void SetSpeed(EWalkSpeedType InType);
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)EWalkSpeedType::Max] = { 200,400,600 };



private:
	float Health;

	bool bCanMove = true;

};
