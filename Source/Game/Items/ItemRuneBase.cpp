#include "ItemRuneBase.h"
#include "Global.h"
#include "Components/CAttachmentStatusComponent.h"

void AItemRuneBase::EquipRune_Implementation()
{
	CLog::Print("Call Equip");
	CLog::Print(BonusSpeed);
	CLog::Print(BonusPower);
	AttachmentStatus->IncreaseAtkSpeed(BonusSpeed);
	AttachmentStatus->IncreasePower(BonusPower);
	AddSideEffect();
}									   

void AItemRuneBase::UnequipRune_Implementation()
{
	CLog::Print("Call Unequip");
	AttachmentStatus->DecreaseAtkSpeed(BonusSpeed);
	AttachmentStatus->DecreasePower(BonusPower);
	RemoveSideEffect();
}