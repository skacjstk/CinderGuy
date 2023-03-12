// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageState.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageState : public UInterface
{
	GENERATED_BODY()
};

class GAME_API IIDamageState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageState")
		void Reflected(float Damage, AController* ParryController, AController* EventInstigator, AActor* DamageCauser);
	virtual void Reflected_Implementation(float Damage, AController* ParryController, AController* EventInstigator, AActor* DamageCauser);	// 이 객체가 반격당했을 때 해야 할 행동 ( 캐릭터는 처맞기, 투사체는 반격하기 등의 행동 )

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageState")
		void Damaged(); // 성공적으로 데미지를 주었을 때 호출
	virtual void Damaged_Implementation();

};
