#include "CDamageEffectComponent.h"
#include "Global.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Characters/ICharacter.h"

UCDamageEffectComponent::UCDamageEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;	
	CHelpers::GetAsset(&emberParticle, TEXT("/Game/M5VFXVOL2/Particles/Fire/Fire_03"));
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCDamageEffectComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UCDamageEffectComponent::DamageEffect(AActor* DamageCauser, EDamageType InType, EDamageEffectType InEffectType)
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
			Server_SetEmberFire();
			FireDamageDel.BindUFunction(this, FName("FireDamage"), DamageCauser);
			GetWorld()->GetTimerManager().SetTimer(FireDamageHandle, FireDamageDel, 1.0f, true);
		}
		break;
	default:
		break;
	}
	return false;
}

void UCDamageEffectComponent::FireDamage(AActor* DamageCauser)
{
	static int index = 0;
	++index;
	if (GetOwner() == nullptr) return;
	if (index > 3 || Cast<IICharacter>(GetOwner())->IsDead())
	{
		index = 0;
		GetWorld()->GetTimerManager().ClearTimer(FireDamageHandle);
		FireDamageHandle.Invalidate();	// 단순히 값을 0으로 바꿈
		Server_RemoveEmberFire();
	}
	else
	{
		GetOwner()->TakeDamage(5.0f, FDamageEvent(), nullptr, DamageCauser);
	}
}

void UCDamageEffectComponent::Server_SetEmberFire_Implementation()
{
	MC_SetEmberFire();
}

void UCDamageEffectComponent::MC_SetEmberFire_Implementation()
{
	if (!!emberParticle)
	{
		FTransform transform;
		transform.SetLocation(GetOwner()->GetActorLocation());
		emberParticleComp = UGameplayStatics::SpawnEmitterAttached(emberParticle, GetOwner()->GetRootComponent(), FName(), transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D(), EAttachLocation::KeepWorldPosition);
		emberParticleComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	} 
}

void UCDamageEffectComponent::Server_RemoveEmberFire_Implementation()
{
	MC_RemoveEmberFire();
}

void UCDamageEffectComponent::MC_RemoveEmberFire_Implementation()
{
	emberParticleComp->Deactivate();
}
