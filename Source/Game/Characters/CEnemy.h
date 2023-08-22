#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class GAME_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void ChangeColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	virtual void Hitted() override;
	virtual void Dead() override;
	UFUNCTION()
		void End_Dead() override;
	virtual bool CheckInvincible() override;

	UFUNCTION()
		void RestoreLogoColor();
protected:
	//	위젯
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;


	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCDamageEffectComponent* DamageEffect;

	UPROPERTY(VisibleDefaultsOnly)
		class UCItemDropComponent* ItemDrop;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	UPROPERTY(EditAnywhere)
		float DeadLaunchValue = 10000.f;
	UPROPERTY(EditAnywhere)
		float LaunchValue = 5.f;	// 밀려나는 고유값
	float DamageValue;
	AActor* Causer;
	ACharacter* Attacker;
};
