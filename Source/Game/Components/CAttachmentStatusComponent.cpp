#include "CAttachmentStatusComponent.h"
#include "Global.h"
#include "Actions/CAttachment.h"

UCAttachmentStatusComponent::UCAttachmentStatusComponent()
{
}

void UCAttachmentStatusComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCAttachmentStatusComponent::InitStatus(FAttachmentStatusData AttachmentStatusData)
{
	CurrentPower = BasePower = AttachmentStatusData.BasePower;
	CurrentAtkSpeed = BaseAtkSpeed = AttachmentStatusData.BaseAtkSpeed;	// 기본스탯 초기화 

	CLog::Print(CurrentPower);
	CLog::Print(CurrentAtkSpeed);
}

void UCAttachmentStatusComponent::IncreasePower(float Amount)
{
	CurrentPower += (BasePower *Amount);
}

void UCAttachmentStatusComponent::DecreasePower(float Amount)
{
	CurrentPower -= (BasePower * Amount);
}

void UCAttachmentStatusComponent::IncreaseAtkSpeed(float Amount)
{
	CurrentAtkSpeed += (BaseAtkSpeed * Amount);
}

void UCAttachmentStatusComponent::DecreaseAtkSpeed(float Amount)
{
	CurrentAtkSpeed -= (BaseAtkSpeed * Amount);
}


