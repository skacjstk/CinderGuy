#include "CActionData.h"
#include "Global.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "CDoAction.h"
#include "CThrow.h"
#include "GameFramework/Character.h"

void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCActionObjectContainer** OutObject)
{
	FTransform transform;

	ACAttachment* Attachment = nullptr;
	ACEquipment* Equipment = nullptr;
	ACDoAction* DoAction = nullptr;
	//CEquipment 생성: Character 만 World가 있기에 
	if (!!AttachmentClass)
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		Attachment->SetActorLabel(GetLabelName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(Attachment, transform);	// 얘는 상속받은 위치에서 AttachTo 함 
	}
	if (!!EquipmentClass)
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		Equipment->SetData(EquipmentData);	// 얘를 호출하고 BeginPlay 가 나와야 해서
		Equipment->SetColor(EquipmentColor);
		Equipment->SetActorLabel(GetLabelName(InOwnerCharacter, "Equipment"));
		UGameplayStatics::FinishSpawningActor(Equipment, transform);	// 여기가 진짜위치
		Equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	}	

	// Action 와 Equipment 전부 생성 성공시, 이벤트 바인딩
	if (!!Attachment && !!Equipment) {
		Equipment->OnEquipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnEquip);
		Equipment->OnUnequipmentDelegate.AddDynamic(Attachment, &ACAttachment::OnUnequip);
	}

	//DoActionObject 생성 
	if (!!DoActionClass)
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
		DoAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		DoAction->SetDatas(DoActionDatas);
		// StrongData 넣기 ( DoAction 하위개념이라 Subclass가 필요 없음
		DoAction->SetStrongData(StrongData);
		DoAction->SetActorLabel(GetLabelName(InOwnerCharacter, "DoAction"));
		UGameplayStatics::FinishSpawningActor(DoAction, transform);

		// 있으면, Equipment의 현재 장착여부 변수 레퍼런스 설정
		if(!!Equipment)
		{ 
			DoAction->SetEquipped(Equipment->IsEquipped());
		}

		if (!!Attachment)
		{
			// DoAction 오버랩 이벤트 바인딩
			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &ACDoAction::OnAttachmentEndOverlap);
//			Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");
		}


	}//end if
	// 최종적으로 UCActionData의 BeginPlay를 호출한 곳의 OutObject에 완성된 결과를 반환. 현재는 CActionComponent::BeginPlay() 에서 호출한다.
	*OutObject = NewObject<UCActionObjectContainer>();
	(*OutObject)->Attachment = Attachment;
	(*OutObject)->Equipment = Equipment;
	(*OutObject)->DoAction = DoAction;		// Friend 먹여놔서 된다. 
	(*OutObject)->EquipmentColor = EquipmentColor;

}

FString UCActionData::GetLabelName(class ACharacter* InOwnerCharacter, FString InMiddleName)
{
	FString name;
	name.Append(InOwnerCharacter->GetActorLabel());
	name.Append("_");
	name.Append(InMiddleName);
	name.Append("_");
	name.Append(GetName().Replace(L"DA_",L""));

	return name;
}