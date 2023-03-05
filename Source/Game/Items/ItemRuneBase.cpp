#include "ItemRuneBase.h"
#include "Global.h"
#include "Components/CAttachmentStatusComponent.h"

void AItemRuneBase::EquipRune_Implementation()
{
	CLog::Print("Call Equip");
	AttachmentStatus->IncreasePower(10.f);
}

void AItemRuneBase::UnequipRune_Implementation()
{
	CLog::Print("Call Unequip");

}