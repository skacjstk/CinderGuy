#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCStatusComponent::UCStatusComponent()
{
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health	= MaxHealth;
	// ...
	
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::SetSpeed(EWalkSpeedType InType)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());
	movement->MaxWalkSpeed = Speed[(int32)InType];
}

void UCStatusComponent::IncreaseHealth(float InAmount)
{
	Health += InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

}
void UCStatusComponent::DecreaseHealth(float InAmount)
{
	Health -= InAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}
