#include "CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Net/UnrealNetwork.h"

void ACThrow::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACThrow, ImpactParticle);
}
ACThrow::ACThrow()
{
	CHelpers::CreateSceneComponent(this, &Sphere, "Sphere");
	CHelpers::CreateSceneComponent(this, &ThrowParticle, "ThrowParticle", Sphere);
	CHelpers::CreateActorComponent(this, &Projectile, "Projectile");

	Projectile->InitialSpeed = 4000.f;
	Projectile->MaxSpeed = 8000.f;
	Projectile->ProjectileGravityScale = 0.f;
	Projectile->bSweepCollision = true;

	InitialLifeSpan = 3.f;
	bReplicates = true;
	Projectile->SetIsReplicated(true);
	ThrowParticle->SetIsReplicated(true);
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACThrow::OnComponentBeginOverlap);	
}

void ACThrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());
	this->FromSweep = bFromSweep;
	LastSweepResult = SweepResult;
	// 폭발 효과 Damaged로 옮김
	if (OnThrowBeginOverlap.IsBound())
		OnThrowBeginOverlap.Broadcast(SweepResult);
}

void ACThrow::Reflected_Implementation(float Damage, AController* ParryController, AController* EventInstigator, AActor* DamageCauser)
{
	// 소유자를 바꿔치기 하기 
	ACThrow* throwCauser = Cast<ACThrow>(DamageCauser);
	if (!!throwCauser)
	{
		// 투사체 소유자 바꾸기
		throwCauser->SetInstigator(ParryController->GetPawn());
		throwCauser->SetOwner(ParryController->GetPawn());

		// 투사체 방향 바꾸기	
		FRotator newVel = UKismetMathLibrary::FindLookAtRotation(ParryController->GetPawn()->GetActorLocation(), EventInstigator->GetPawn()->GetActorLocation());
		throwCauser->Projectile->Velocity = newVel.Vector().GetSafeNormal() * throwCauser->Projectile->InitialSpeed;
	}
}

void ACThrow::Damaged_Implementation()
{
	if (!!ImpactParticle)
	{
		FTransform	transform = ImpactParticleTransform;
		// 폭발 이펙트 재생위치 보정하기 sweep 됬으면 충돌된 위치 vs 원래 있던 투사체의 위치 
		transform.AddToTranslation(this->FromSweep ? LastSweepResult.Location : GetActorLocation());
		transform.SetRotation(FQuat(LastSweepResult.ImpactNormal.Rotation()));
		Server_ImpactParticle(transform);
	}
	Destroy();
}

void ACThrow::Server_ImpactParticle_Implementation(FTransform transform)
{
	CHelpers::GetRoleText(GetLocalRole());
	MC_ImpactParticle(transform);
}

void ACThrow::MC_ImpactParticle_Implementation(FTransform transform)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, transform);
}

/*
https://nerivec.github.io/old-ue4-wiki/pages/reflecting-projectile-c.html
이건 벽에 튕기는 코드같은데 흥미로워서 넣어놈
	// Reflect the projectile because we hit a non-physics object
	FVector ReflectedVelocity = fBounceSpeedLoss * (-2 * FVector::DotProduct(MyVelocity, HitNormal) * HitNormal + MyVelocity);
	MyVelocity = ReflectedVelocity;
	ReflectedVelocity.Normalize();
	SetActorRotation(ReflectedVelocity.Rotation());
*/