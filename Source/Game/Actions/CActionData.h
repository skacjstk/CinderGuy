#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CActionObjectContainer.h"
#include "CActionData.generated.h"

UCLASS()
class GAME_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
		
public:
	void BeginPlay(class ACharacter* InOwnerCharacter, UCActionObjectContainer** OutObject);	// 얘는 BeginPlay가 없어서 직접 만듬;

private:
	FString GetLabelName(class ACharacter* InOwnerCharacter, FString InMiddleName);
public:
	//Attachment
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attachment")
		TSubclassOf<class ACAttachment> AttachmentClass;	// 무기 Mesh, Attach 담당

	// Equipment
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		TSubclassOf<class ACEquipment> EquipmentClass;	// 무기 내부 장착 몽타주 담당 
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		FEquipmentData EquipmentData;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Equipment")
		FLinearColor EquipmentColor;	// 무기에 맞게 마네킹 몸 색 바꾸기 

	// DoAction
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DoAction")
		TSubclassOf<class ACDoAction> DoActionClass;	// 무기 내부 공격 몽타주 및 데미지 담당
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DoAction")
		TArray<FDoActionData> DoActionDatas;
		
};
