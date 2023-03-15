#include "KatanaParryDamageType.h"
#include "Global.h"

void UKatanaParryDamageType::DamageTrigger_Implementation(UCDamageEffectComponent* DamageEffectComp)
{
	CLog::Print("Called Katana");
	if (!!DamageEffectComp)
		DamageEffectComp->DamageEffect(EDamageType::Slash, EDamageEffectType::Parry);

}