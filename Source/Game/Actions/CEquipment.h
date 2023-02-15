// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);

UCLASS()
class GAME_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACEquipment();

	// 따로구현 해야하는데 귀찮
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }
	FORCEINLINE const bool* IsEquipped() { return &bEquipped; }

	FORCEINLINE FEquipmentData GetData() { return Data; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// 장착하면서 발생해야할 추가 기능은 BP에서 구현 
	UFUNCTION(BlueprintNativeEvent)
		void Equip();	// 장착 몽타주 재생 함수 _Implementation
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();	
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();	// 장착 몽타주 재생 함수 _Implementation
	void Unequip_Implementation();

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;
	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;
	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;	// 상태를 Equip으로 변경해야 함 
	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;	// CanMove를 여기에 저장해놨음 
private:
	FEquipmentData Data;	// ActionData 에 있음
	FLinearColor Color;
	bool bEquipped;	// 장착 완료 표시

};
