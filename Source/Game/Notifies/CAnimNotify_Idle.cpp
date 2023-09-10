#include "CAnimNotify_Idle.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_Idle::GetNotifyName_Implementation() const
{
	return "SetIdle";
}

void UCAnimNotify_Idle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(MeshComp->GetOwner());
	CheckNull(state);
	CheckNull(status);
	status->SetMove();

	if (MeshComp->GetOwner()->HasAuthority() == true)
		state->SetIdleMode();
}