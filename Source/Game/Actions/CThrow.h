#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IDamageState.h"
#include "CThrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThrowBeginOverlap, FHitResult, InHitResult);

UCLASS()
class GAME_API ACThrow : public AActor , public IIDamageState
{
	GENERATED_BODY()
	
public:	
	ACThrow();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void Reflected_Implementation(float Damage, AController* ParryController, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Damaged_Implementation() override;

	class TSubclassOf<UDamageType> GetDamageType() { return ThrowDamageType; }
public:
	UPROPERTY(BlueprintAssignable)
		FThrowBeginOverlap OnThrowBeginOverlap;

private:
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* ImpactParticle;
	UPROPERTY(EditDefaultsOnly)
		FTransform ImpactParticleTransform;
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* ThrowParticle;	//  충돌체 밑에 붙일거라 컴포넌트
	UPROPERTY(EditDefaultsOnly)
		class UProjectileMovementComponent* Projectile;	// BP 상속
	UPROPERTY(EditDefaultsOnly)
		class TSubclassOf<UDamageType> ThrowDamageType;	// 데미지 타입 클래스
	
private:
	bool FromSweep;
	FHitResult LastSweepResult;
	
};
