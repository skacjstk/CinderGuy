#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/CDamageEffectComponent.h"
#include "IDamageType.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageType : public UInterface
{
	GENERATED_BODY()
};

class GAME_API IIDamageType
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "DamageType")
		void DamageTrigger(UCDamageEffectComponent* DamageEffectComp);
	virtual void DamageTrigger_Implementation(UCDamageEffectComponent* DamageEffectComp);
};
