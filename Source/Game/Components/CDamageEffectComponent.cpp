#include "CDamageEffectComponent.h"
#include "Global.h"

UCDamageEffectComponent::UCDamageEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCDamageEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UCDamageEffectComponent::DamageEffect(EDamageType InType, EDamageEffectType InEffectType)
{
	CLog::Print("Component Call");
	//Todo: 여기서 받아야 할 Enum, 그 Enum 간의 처리를 구현하기
	switch (InType)
	{
	case EDamageType::None:
		break;
	case EDamageType::Slash:
		CLog::Print("Slash");
		break;
	case EDamageType::Magic:
		break;
	case EDamageType::MAX:
		break;
	default:
		break;
	}
	switch (InEffectType)
	{
	case EDamageEffectType::Parry:
		 // Owner의 Reflection 호출
		// Causer 의 그게 필요하다 
		break;
	default:
		break;
	}

	return false;
}
