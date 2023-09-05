#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CActionObjectContainer.generated.h"

UENUM(BlueprintType)
enum class EStrongActionEndType : uint8
{
	Once, Doing, MAX
};

UENUM(BlueprintType)
enum class EGuardActionType : uint8
{
	Block, Parry, MAX
};
/*
@ struct Equipment
*/
USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.f;
	UPROPERTY(EditAnywhere)
		FName StartSection = "";
	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;	// LookForward: true라면, 정면 고정, orientRotation을 사용하지 않을 것
};
/*
@ struct DoStrongAction
*/
USTRUCT(BlueprintType)
struct FGuardData : public FEquipmentData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EGuardActionType GuardType;	// 패리액션, 가드백 액션 용
};

/*
@ struct DoAction
*/
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData	// 상속받았기에, DataAsset에선 아래쪽에 얘가 보임
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		float Power = 1.f;
	UPROPERTY(EditAnywhere)
		float HitStop;	// 멈칫 시간 
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;
	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShake> ShakeClass;
	UPROPERTY(EditAnywhere)
		FString SpecificCollisionName = L"None";
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;	// 액션 수행시 던져질 아이템
};
/*
@ struct DoStrongAction
*/
USTRUCT(BlueprintType)
struct FDoStrongActionData : public FDoActionData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStrongActionEndType EndType;	// 강공격이 단타인지, 아닌지
	UPROPERTY(EditAnywhere)
		FName EndSection = L"End";
};


/*
@ struct FAttachmentStatus
*/
USTRUCT(BlueprintType)
struct FAttachmentStatusData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		float BasePower = 1.0f;
	UPROPERTY(EditAnywhere)
		float BaseAtkSpeed = 1.0f;
};

/*------------------------------------------------------------------------
* @ class Action Object Container
------------------------------------------------------------------------*/
UCLASS()
class GAME_API UCActionObjectContainer : public UObject
{
	GENERATED_BODY()
public:
	friend class UCActionData;	// UCActionData에서 this를 모두 공개한다. 
	friend class UCAttachmentStatusComponent;
public:
	UCActionObjectContainer();
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }	// Attachment 클래스 가져오기: 이 Attachment 속 Inventory 가져오면 성공
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE FLinearColor GetEquipmentColor() { return EquipmentColor; }
	FORCEINLINE class ACharacter* GetOwnerCharacter() { return character; }
	FORCEINLINE void SetOwnerCharacter(class ACharacter* InCharacter) { this->character = InCharacter; }

private:	// 실제 객체
	class ACAttachment* Attachment;
	class ACEquipment* Equipment;
	class ACDoAction* DoAction;
	FLinearColor EquipmentColor;
	class ACharacter* character;
};
