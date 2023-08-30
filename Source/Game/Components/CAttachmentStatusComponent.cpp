#include "CAttachmentStatusComponent.h"
#include "Global.h"
#include "Actions/CAttachment.h"

UCAttachmentStatusComponent::UCAttachmentStatusComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCAttachmentStatusComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCAttachmentStatusComponent::InitStatus(FAttachmentStatusData AttachmentStatusData)
{
	CurrentPower = BasePower = AttachmentStatusData.BasePower;
	CurrentAtkSpeed = BaseAtkSpeed = AttachmentStatusData.BaseAtkSpeed;	// 기본스탯 초기화
}

void UCAttachmentStatusComponent::IncreasePower(float Percent)
{
	CurrentPower += (BasePower *Percent);
}

void UCAttachmentStatusComponent::DecreasePower(float Percent)
{
	CurrentPower -= (BasePower * Percent);
}

void UCAttachmentStatusComponent::IncreaseAtkSpeed(float Percent)
{
	CurrentAtkSpeed += (BaseAtkSpeed * Percent);
}

void UCAttachmentStatusComponent::DecreaseAtkSpeed(float Percent)
{
	CurrentAtkSpeed -= (BaseAtkSpeed * Percent);
}


