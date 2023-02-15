#include "CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "Components/CActionComponent.h"
#include "Actions/CDoAction_Melee.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "CanCombo";
}
void UCAnimNotifyState_Combo::NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);

	CheckNull(MeshComp->GetOwner());	// 임시조치
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetDoAction());
	CheckNull(melee);
	melee->EnableCombo();
}
void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);

	CheckNull(MeshComp->GetOwner());	// 임시조치
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(MeshComp->GetOwner());
	CheckNull(action);

	ACDoAction_Melee* melee = Cast<ACDoAction_Melee>(action->GetCurrent()->GetDoAction());
	CheckNull(melee);
	melee->DisableCombo();
}