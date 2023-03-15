#include "CDamageEffectComponent.h"
#include "Global.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

UCDamageEffectComponent::UCDamageEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
	CHelpers::GetAsset(&emberParticle, TEXT("/Game/M5VFXVOL2/Particles/Fire/Fire_03"));
}


// Called when the game starts
void UCDamageEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UCDamageEffectComponent::DamageEffect(EDamageType InType, EDamageEffectType InEffectType)
{
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
			SetEmberFire();
			FireDamageHandle = UKismetSystemLibrary::K2_SetTimer(this, "FireDamage", 1.0f, true);
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
	++index;
	GetOwner()->TakeDamage(5.0f, FDamageEvent(), nullptr, nullptr);

	if (index >= 3)
	{
		index = 0;
		GetWorld()->GetTimerManager().ClearTimer(FireDamageHandle);
		FireDamageHandle.Invalidate();	// 단순히 값을 0으로 바꿈
		RemoveEmberFire();
	}
}

void UCDamageEffectComponent::SetEmberFire()
{
	if (!!emberParticle)
	{
		FTransform transform;
		transform.SetLocation(GetOwner()->GetActorLocation());
		emberParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), emberParticle, transform);
	}
}

void UCDamageEffectComponent::RemoveEmberFire()
{
	emberParticleComp->Deactivate();
}
