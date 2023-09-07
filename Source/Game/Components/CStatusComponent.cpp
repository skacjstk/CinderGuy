#include "CStatusComponent.h"
#include "Global.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CEnemy.h"

// Sets default values for this component's properties
UCStatusComponent::UCStatusComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCStatusComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCStatusComponent, Health);
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

void UCStatusComponent::ServerHealthUpdate_Implementation(float ChangeAmountHealth)
{
	Health = FMath::Clamp((Health + ChangeAmountHealth), 0.0f, MaxHealth);	
	// �Ϲ��Լ��� AUthority ���Ѱɾ �Ǳ�� �� ������ ���� ó���ϴ°� ������ �����°� �����ϴٰ� �Ǵ�����
}

void UCStatusComponent::OnRep_UpdateHealth()
{
	if(OnUpdateHealth.IsBound())
		OnUpdateHealth.Broadcast(Health);
}

void UCStatusComponent::IncreaseHealth(float InAmount)
{
	ServerHealthUpdate(InAmount);
}
void UCStatusComponent::DecreaseHealth(float InAmount)
{
	ServerHealthUpdate(-InAmount);
}
