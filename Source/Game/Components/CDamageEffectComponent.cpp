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
	//Todo: 여기서 받아야 할 Enum, 그 Enum 간의 처리를 구현하기
	CLog::Print("DamageEffect Call");
	switch (InType)
	{
	case EDamageType::None:
		break;
	case EDamageType::Slash:
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
	case EDamageEffectType::Fire:	// 1초마다 5의 피해를 3번 주는 FireDamage 만들기
		if (!FireDamageHandle.IsValid()) {
			CLog::Print("FireDamage Call");
			FireDamageHandle = UKismetSystemLibrary::K2_SetTimer(this, "FireDamage", 1.0f, true, 1.0f);
			// Todo: 상태이상 Component 만들기 
		}
		break;
	default:
		break;
	}
	return false;
}

void UCDamageEffectComponent::FireDamage()
{
	static int index = 0;
	CLog::Print("FireDamage");
	++index;
	GetOwner()->TakeDamage(5.0f, FDamageEvent(), nullptr, nullptr);

	if (index >= 3)
	{
		index = 0;
		GetWorld()->GetTimerManager().ClearTimer(FireDamageHandle);
		FireDamageHandle.Invalidate();	// 단순히 값을 0으로 바꿈
	}
}
