#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CInventoryComponent.h"
#include "CActionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, MagicBall, Storm, Katana, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

/*
@ struct FSaveActionData
*/
USTRUCT(BlueprintType)
struct FSaveActionData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EActionType ActionType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FSlot> AttachmentInventroySlot;	// Todo: attachment Data도 저장하기
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCActionObjectContainer* GetCurrent() { return DataObjects[(int32)Type]; } // 실제 메모리 데이터로 변경
	// Anvil, 룬 변경 기능을 위한 배열 전체 반환
	UFUNCTION(BlueprintPure)
		class UCActionObjectContainer* GetDataObject(int32 InIndex) { return DataObjects[InIndex]; } // Attachment 가져오기
	UFUNCTION(BlueprintPure)
		int32 GetActionIndex() { return (int32)EActionType::Max; }
public:
	//ActionType Is 
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsStormMode() { return Type == EActionType::Storm; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsKatanaMode() { return Type == EActionType::Katana; }
public:
	// ActionType Set								 
	UFUNCTION(BlueprintCallable) void SetUnarmedMode();								 
	UFUNCTION(BlueprintCallable) void SetFistMode();								 
	UFUNCTION(BlueprintCallable) void SetOneHandMode();								 
	UFUNCTION(BlueprintCallable) void SetTwoHandMode();						 
	UFUNCTION(BlueprintCallable) void SetTwoHandModeLate();						 
	UFUNCTION(BlueprintCallable) void SetWarpMode();								 
	UFUNCTION(BlueprintCallable) void SetMagicBallMode();								 
	UFUNCTION(BlueprintCallable) void SetStormMode();
	UFUNCTION(BlueprintCallable) void SetKatanaMode();

public:
	void LateEquip();


	UFUNCTION(Server, Reliable)
	void Server_DoAction();
	void Server_DoAction_Implementation();
	UFUNCTION(NetMulticast, Reliable)
		void MC_DoAction();
	void MC_DoAction_Implementation();

	void DoStrongAction();	
	UFUNCTION(Server, Reliable)
		void Server_DoStrongAction();
	void Server_DoStrongAction_Implementation();
	UFUNCTION(NetMulticast, Reliable)
		void MC_DoStrongAction();
	void MC_DoStrongAction_Implementation();

	void EndDoStrongActionWait();	// 강공격 해제 대기
	UFUNCTION(Server, Reliable)
		void Server_EndDoStrongActionWait();
	void Server_EndDoStrongActionWait_Implementation();
	UFUNCTION(NetMulticast, Reliable)
		void MC_EndDoStrongActionWait();
	void MC_EndDoStrongActionWait_Implementation();

//	void DestroyOwningWeapon();
	UFUNCTION(Server, Reliable)
		void Server_DestroyOwningWeapon();
	void Server_DestroyOwningWeapon_Implementation();

	UFUNCTION(NetMulticast, Reliable)
		void MC_DestroyOwningWeapon();
	void MC_DestroyOwningWeapon_Implementation();

	void DoOnAim();		// 우클릭 누르기
	void DoOffAim();	// 우클릭 떼기

	void DoParry();	// 패리
	void DoBlock();	// 블로킹
	//	사망 시 충돌체 끄기 외 사망했을 때 했어야 할 일을 호출 (컴포넌트 속 has 관계 아이템 제거 등)
	void Dead();
	void End_Dead();


	void OffAllCollisions();

	void AbortByDamaged();
private:
	UFUNCTION(Reliable, Server)
		void Server_SetMode(EActionType InNewType);
	void Server_SetMode_Implementation(EActionType InNewType);
	UFUNCTION(NetMulticast, Reliable)
		void MC_SetMode(EActionType InNewType);
	void MC_SetMode_Implementation(EActionType InNewType);

	UFUNCTION(Reliable, Server)
	void Server_ChangeType(EActionType InNewType);
	void Server_ChangeType_Implementation(EActionType InNewType);
	UFUNCTION(NetMulticast, Reliable)
		void MC_ChangeType(EActionType InNewType);
	void MC_ChangeType_Implementation(EActionType InNewType);

	// 필드
public:
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;
private:
	UPROPERTY(Replicated)
		EActionType Type;
private:
	UPROPERTY(EditDefaultsOnly)
		class UCActionData* Datas[(int32)EActionType::Max];	// DataAsset: 세팅값 포함 
	UPROPERTY(Replicated)	// 내부 Heap할당 하기 때문에 넣어줌 
		class UCActionObjectContainer* DataObjects[(int32)EActionType::Max];	// Data 실제 객체
};
