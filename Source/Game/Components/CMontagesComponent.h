// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "CStateComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;
	UPROPERTY(EditAnywhere)
		FName StartSection = NAME_None;
	UPROPERTY(EditAnywhere)
		bool bCanMove;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMontagesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void PlayRoll();
	void PlayBackStep();
	void PlayHitted();
	void PlayDead();

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataTable")
		UDataTable* DataTable;

	void PlayAnimMontage(EStateType InType);
private:
	FMontageData* Datas[(int8)EStateType::Max];	// 실제 데이터 담길 배열 

};
