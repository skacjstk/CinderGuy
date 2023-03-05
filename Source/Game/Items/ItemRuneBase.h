#pragma once

#include "CoreMinimal.h"
#include "CItemBase.h"
#include "Engine/DataTable.h"
#include "ItemRuneBase.generated.h"


USTRUCT(BlueprintType)
struct FRune: public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName RuneName;	// ItemName 과 일치해야 한다.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float BonusSpeed;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float BonusPower;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText SideEffectDesc;	// 하드코딩: 특수효과는 BP에서 구현
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<AItemRuneBase> RuneClass;
};

UCLASS()
class GAME_API AItemRuneBase : public ACItemBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "RuneEffect")
		void EquipRune();
	virtual	void EquipRune_Implementation();
	UFUNCTION(BlueprintNativeEvent, Category = "RuneEffect")
		void UnequipRune();
	virtual void UnequipRune_Implementation();
	UFUNCTION(BlueprintImplementableEvent, Category = "RuneEffect")
		void AddSideEffect();	// 부가 효과 ex) 공격시, 회피 시 등등을 바운딩
	UFUNCTION(BlueprintImplementableEvent, Category = "RuneEffect")
		void RemoveSideEffect();	// 부가 효과 제거
public:
	void SetBonusSpeed(float InBonusSpeed) { BonusSpeed = InBonusSpeed; }
	void SetBonusPower(float InBonusPower) { BonusPower = InBonusPower; }

public:
	class UCAttachmentStatusComponent* AttachmentStatus;	// SpawnActorDefferd 로 할당 후 호출

private:
	float BonusSpeed;
	float BonusPower;
	FText SideEffectDesc;
};
