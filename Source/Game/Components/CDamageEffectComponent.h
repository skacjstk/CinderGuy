#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDamageEffectComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None, Slash, Magic, MAX
};

UENUM(BlueprintType)
enum class EDamageEffectType : uint8
{
	Normal, Parry, Fire, Firing, MAX
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCDamageEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCDamageEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "DamageEffectComponent")
		bool DamageEffect(class AActor* DamageCauser, EDamageType InType, EDamageEffectType InEffectType = EDamageEffectType::Normal);
		
private:
	UFUNCTION()
		void FireDamage(class AActor* DamageCauser);
	UFUNCTION(Reliable, Server)
		void Server_SetEmberFire();
		void Server_SetEmberFire_Implementation();
	UFUNCTION(Reliable, NetMulticast)
		void MC_SetEmberFire();
		void MC_SetEmberFire_Implementation();

	UFUNCTION(Reliable, Server)
		void Server_RemoveEmberFire();
	void Server_RemoveEmberFire_Implementation();

	UFUNCTION(Reliable, NetMulticast)
		void MC_RemoveEmberFire();
	void MC_RemoveEmberFire_Implementation();

	FTimerHandle FireDamageHandle;
	FTimerDelegate FireDamageDel;
	UPROPERTY(EditAnywhere, Category = "DamageEffectParticle")
		UParticleSystem* emberParticle;
	UParticleSystemComponent* emberParticleComp;
};
