#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "ICharacter.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class GAME_API ACPlayer : public ACharacter, public IICharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

private:
	// SceneComponent
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;
	// ActorComponent
	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;
	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;
	UPROPERTY(VisibleDefaultsOnly)
	class UCMontagesComponent* Montages;
	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;
	UPROPERTY(VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;
	UPROPERTY(VisibleDefaultsOnly)
		class UCDamageEffectComponent* DamageEffect;

	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
	class UCWidget_PlayerHUD* PlayerHUD;

	TSubclassOf<class UUserWidget> DefaultHUDClass;	// 왜인지 subclass 위젯은 죄다 
	// 강공격을 구현하기 위한 
	APlayerController* Controller;
	FKey ActionMapKey;	// "Action" 과 연결된 키 구하기
	FKey AimMapKey;	// "Aim" 과 연결된 키 구하기(우클릭)

	UPROPERTY(EditDefaultsOnly)
		uint8 TeamID = 0;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FGenericTeamId GetGenericTeamId() const override;	// Interface 재정의 

	// Todo: 이걸 Controller에 넣을까 고민중
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class UCWidget_PlayerHUD* GetPlayerHUD() { return PlayerHUD; }
private:
	//Axis Mapping
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);

	// Action Mapping
	void OnEvade();
	void OnWalk();
	void OffWalk();

	void OnFist();
	void OnOneHand();
	void OnTwoHand();
	void OnMagicBall();
	void OnWarp();
	void OnKatana();

	void OnDoAction();	// 좌클릭
	void OnDoStrongAction(); // 좌클릭 0.5초 이상 누르기
	void OffDoAction();	// 강공격 해제용

	void OnAim();	// 우클릭
	void OffAim();

	void OnGuard();	// StateTypeChanged 에 

	// Interact
	void OnInteract();
	void OnTestInventory();
private:
	void Begin_Roll();
	void Begin_BackStep();
public:
	void End_Roll();
	void End_BackStep();	// 거의 notify 호출 
	void End_Parry();	// notify 호출

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Hitted() override;
	virtual void Dead() override;
	virtual void End_Dead() override;
	virtual bool CheckInvincible() override;
	// IICharacter을(를) 통해 상속됨
	virtual void ChangeColor(FLinearColor InColor);
private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);	
	// CStateComponent에 OnStateTypeChanged에 바인딩
	UFUNCTION()
		void GuardAlpha(float Output);

private:
	float DamageValue;
	AActor* Causer;
	ACharacter* Attacker;

	// 가드에 필요한 거
	class UCurveFloat* Curve;
	FOnTimelineFloat TimelineFloat;
	FTimeline GuardTimeline;
};
