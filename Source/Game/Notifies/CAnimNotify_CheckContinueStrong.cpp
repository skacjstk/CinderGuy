#include "CAnimNotify_CheckContinueStrong.h"
#include "Global.h"
#include "Components/CActionComponent.h"
#include "Actions/CAttachment.h"
#include "Actions/CActionData.h"
#include "Actions/CDoAction_Melee.h"

FString UCAnimNotify_CheckContinueStrong::GetNotifyName_Implementation() const
{
	return "CheckStrongAction";
}
void UCAnimNotify_CheckContinueStrong::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	action->GetCurrent()->GetDoAction()->CheckEndDoStrongAction();	// 주기적인 반복 검사
}