#include "CActionObjectContainer.h"
#include "Components/CActionComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void ACActionObjectContainer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACActionObjectContainer, Attachment);
	DOREPLIFETIME(ACActionObjectContainer, Equipment);
	DOREPLIFETIME(ACActionObjectContainer, DoAction);
	DOREPLIFETIME(ACActionObjectContainer, EquipmentColor);
}
ACActionObjectContainer::ACActionObjectContainer()
{
	SetReplicates(true);
}