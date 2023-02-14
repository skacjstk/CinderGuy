// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class GAME_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDoAction();

	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetStrongData(FDoStrongActionData InData) { StrongData = InData; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() {};
	virtual void DoStrongAction() {};	// 강공격
	virtual void EndDoStrongAction() {};
	virtual void Begin_DoAction() {};
	virtual void End_DoAction() {};

	virtual void OnAim() {};
	virtual void OffAim() {};

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) {};
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) {};
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	TArray<FDoActionData> Datas;
	FDoStrongActionData StrongData;	// Melee 는 강공격이 추가되었음
	const bool* bEquipped;	//CEquipment 에서 받아올 것
};
