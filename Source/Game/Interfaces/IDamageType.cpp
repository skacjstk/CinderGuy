#include "IDamageType.h"

void IIDamageType::DamageTrigger_Implementation(UCDamageEffectComponent* DamageEffectComp)
{
}

void IIDamageType::GetHitResult(FHitResult** OutHitResult)
{
	OutHitResult = &SweepResult;
}

void IIDamageType::SetHitResult(const FHitResult& InHitResult)
{
	FHitResult& hitResult = const_cast<FHitResult&>(InHitResult);
	SweepResult = &hitResult;
}
