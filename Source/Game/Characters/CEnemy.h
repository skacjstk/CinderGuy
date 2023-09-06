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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	ACEnemy();
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	void ChangeColor(FLinearColor InColor) override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType, class AActor* DamageCauser);
	UFUNCTION()
		void WidgetHeatlhUpdate(float NewHealth);
	virtual void Hitted(class AActor* DamageCauser = nullptr) override;
	virtual void Dead(class AActor* DamageCauser = nullptr) override;
	UFUNCTION()
		void End_Dead() override;
	virtual bool CheckInvincible() override;

	UFUNCTION()
		void RestoreLogoColor();
	UFUNCTION()
		void SpawnDamageText();
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

	UPROPERTY(VisibleDefaultsOnly)
		class UCDismembermentComponent* SliceBodyEffect;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	UPROPERTY(EditAnywhere)
		float DeadLaunchValue = 10000.f;
	UPROPERTY(EditAnywhere)
		float LaunchValue = 5.f;	// 밀려나는 고유값
	UPROPERTY(Replicated)
	float DamageValue;
	//AActor* Causer;
	ACharacter* Attacker;
};
