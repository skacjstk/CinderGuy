#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Interfaces/IDamageType.h"
#include "DamageTypeBase.generated.h"

UCLASS()
class GAME_API UDamageTypeBase : public UDamageType , public IIDamageType
{
	GENERATED_BODY()
public:
	virtual void DamageTrigger_Implementation(class AActor* DamageCauser, UCDamageEffectComponent* DamageEffectComp) override;

};
