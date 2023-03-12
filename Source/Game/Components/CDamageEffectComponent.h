#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDamageEffectComponent.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None, Slash, Magic, MAX
};
enum class EDamageEffectType : uint8
{
	Normal,Parry, MAX
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
	bool DamageEffect(EDamageType InType, EDamageEffectType InEffectType = EDamageEffectType::Normal);
		
};
