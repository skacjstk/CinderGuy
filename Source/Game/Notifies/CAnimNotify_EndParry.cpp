#include "CAnimNotify_EndParry.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_EndParry::GetNotifyName_Implementation() const
{
	return "EndParry";
}

void UCAnimNotify_EndParry::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Parry();	// Todo: 임시조치
}
