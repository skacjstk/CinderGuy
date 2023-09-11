#include "KatanaParryDamageType.h"
#include "Global.h"

void UKatanaParryDamageType::DamageTrigger_Implementation(AActor* DamageCauser, UCDamageEffectComponent* DamageEffectComp)
{
	if (!!DamageEffectComp)
		DamageEffectComp->DamageEffect(DamageCauser, EDamageType::Slash, EDamageEffectType::Parry);
}