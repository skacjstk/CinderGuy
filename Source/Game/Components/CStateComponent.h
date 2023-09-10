// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/CommonStructs.h"
#include "CStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType, class AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetIdleMode(bool FromServer = false);
	void SetRollMode();
	void SetBackStepMode();
	void SetEquipMode(bool FromServer = false);
	void SetActionMode(bool FromServer = false);
	void SetGuardMode();
	void SetBlockMode();
	void SetParryMode();
	void SetStrongActionMode(bool FromServer = false);
	void SetEndingStrongActionMode();
	void SetHittedMode(class AActor* DamageCauser = nullptr);
	void SetDeadMode(class AActor* DamageCauser = nullptr);

	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	FORCEINLINE bool IsBackMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsGuardMode() { return Type == EStateType::Guard; }
	FORCEINLINE bool IsBlockMode() { return Type == EStateType::GuardBack; }
	FORCEINLINE bool IsParryMode() { return Type == EStateType::Parry; }
	FORCEINLINE bool IsStrongActionMode() { return Type == EStateType::StrongAction; }
	FORCEINLINE bool IsEndingStrongActionMode() { return Type == EStateType::EndingStrongAction; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	UFUNCTION(Server, Unreliable)
		void Server_SetGuardAlpha(float OutAlpha);
	void Server_SetGuardAlpha_Implementation(float OutAlpha) { IFGuardAlpha = OutAlpha; }

	float GetGuardAlpha() { return IFGuardAlpha; }

	float GetGuardFrame() { return IFGuard; }
private:
	UFUNCTION(Server, Reliable)
		void Server_ChangeStateType(EStateType InNewType, class AActor* DamageCauser = nullptr);
		void Server_ChangeStateType_Implementation(EStateType InNewType, class AActor* DamageCauser = nullptr);

		void ChangeType(EStateType InNewType, class AActor* DamageCauser = nullptr);
	UFUNCTION(NetMulticast, Reliable)
		void MC_ChangeStateType(EStateType InNewType, class AActor* DamageCauser = nullptr);
		void MC_ChangeStateType_Implementation(EStateType InNewType, class AActor* DamageCauser = nullptr);

		UFUNCTION()
			void OnRep_ChangeType();

public:
	UPROPERTY(BlueprintAssignable)	// BP 이벤트 꽂을 수 있음
		FStateTypeChanged OnStateTypePreChanged;
	UPROPERTY(BlueprintAssignable)	// BP 이벤트 꽂을 수 있음
		FStateTypeChanged OnStateTypeChanged;
private:
	UPROPERTY(ReplicatedUsing = "OnRep_ChangeType")
		EStateType Type;

	UPROPERTY(EditAnywhere, Category = "Invincible")
		float IFGuard = 0.15f;	// I-Frame Guard 라는 뜻, Parry Alpha 값
	UPROPERTY(EditAnywhere, Category = "Invincible")
		float IFEvade = 0.2f;	// I-Frame Evade, Evade 무적시간 이라는 뜻

	UPROPERTY(Replicated)
		float IFGuardAlpha = 0.0f;	// 가드 알파값 ( 어떤 TimeLine 에 의해 1~ 감소할 것 )
	UPROPERTY(Replicated)
		float IFEvadeAlpha = 0.0f;	// 구르기 알파값 ( 어떤 TimeLine 에 의해 1~ 감소할 것 )
};
