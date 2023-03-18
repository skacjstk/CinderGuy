#include "ItemRuneBase.h"
#include "Global.h"
#include "Components/CAttachmentStatusComponent.h"

AItemRuneBase::AItemRuneBase()
{
//	UStaticMesh* mesh;
//	CHelpers::GetAsset(&mesh, TEXT("StaticMesh'/Game/Meshes/Rock03.Rock03'"));
//	ItemMesh->SetStaticMesh(mesh);
//	ItemMesh->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ItemMesh->SetSimulatePhysics(false);
}
void AItemRuneBase::EquipRune_Implementation()
{
	CLog::Print("Call Equip");
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