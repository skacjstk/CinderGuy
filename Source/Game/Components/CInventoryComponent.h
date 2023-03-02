#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/Texture2D.h"
#include "Interfaces/IInteract.h"
#include "Engine/DataTable.h"
#include "Items/CItemBase.h"
#include "Items/CItemEffectBase.h"
#include "CInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdated);

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName ItemID;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 Quantity;
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase	// Editor에서 인식하게 하기 위해
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText Name;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText Description;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UTexture2D* Thumbnail;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<ACItemBase> ItemClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 StckSize;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<ACItemEffectBase> ItemEffectClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCInventoryComponent : public UActorComponent , public IIInteract
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UCInventoryComponent();

	void InteractionTrace();
	bool AddToInventory(FName InitemID, int32 InQuantity, int32& OutQuantityRemaining);	// 인벤토리에 아이템 추가
	int32 FindSlot(FName& itemID, bool& OutFoundSlot);		// 이미 있는 슬롯인지 검사
	UFUNCTION(BlueprintPure)
		int32 GetMaxStackSize(FName& itemID);		// 해당 아이템의 최대 갯수 구하기

	void AddToStack(int32 index, int32 quantity);	// 정해진 갯수만큼 index에 해당하는 Slot에 아이템 추가
	UFUNCTION(BlueprintCallable)	
		bool AnyEmptySlotsAvailable(int32& OutEmptyIndex);	// 아무데나 빈 칸이 있는지?
	UFUNCTION(BlueprintCallable)
	bool CreateNewStack(FName& InItemID, int32 InQuantity, bool IsVerified, int32 InVerifyIndex);

	FORCEINLINE int32 GetInventorySize() { return InventorySize; }
	FORCEINLINE void SetInventorySize(int32 newSize) { InventorySize = newSize; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ToggleInventroy();
	void TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex);

public:
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Slot")
		void Server_TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex);
	void Server_TransferSlots_Implementation(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex);

	UFUNCTION(Reliable, Server)	// 서버에서 실행: 
		void Server_Interact(class AActor* Target);
	void Server_Interact_Implementation(class AActor* Target);

	UFUNCTION(Client, Unreliable)
		void OnLocalInteract(AActor* Target, AActor* Interactor);
	void OnLocalInteract_Implementation(AActor* Target, AActor* Interactor);

	void DEBUG_PrintContents();
	UFUNCTION(Reliable, NetMulticast)
		void MC_Update();	// 델리게이트 호출
	void MC_Update_Implementation();	// 델리게이트 호출

	void RemoveFromInventory(int32 InSourceIndex, bool IsRemoveWhole, bool IsConsumed);
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Slot")
		void Server_RemoveItem(int32 InSourceIndex,  bool IsRemoveWhole, bool IsConsumed);
	void Server_RemoveItem_Implementation(int32 InSourceIndex, bool IsRemoveWhole, bool IsConsumed);

	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Slot")
		void Server_DropItem(FName InItemID, int32 InQuantity);
	void Server_DropItem_Implementation(FName InItemID, int32 InQuantity);

	// 아이템 소비하기
	UFUNCTION(BlueprintCallable, Reliable, Server, Category = "Slot")
		void Server_ConsumeItem(FName InItemID);
	void Server_ConsumeItem_Implementation(FName InItemID);

	UFUNCTION(BlueprintCallable, Category = "Slot")
		void ConsumeItem(int32 index);	// Action Widget에서 호출

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

	// IInteract 상속
	virtual bool InteractWith_Implementation(class ACharacter* playerCharacter) override;

public:
	UPROPERTY(BlueprintAssignable, Replicated)
		FInventoryUpdated OnInventoryUpdated;		// 델리게이트
	UPROPERTY(BlueprintReadWrite, Replicated)
		TArray<FSlot> Content;
private:
	UPROPERTY(EditDefaultsOnly)
		int32 InventorySize;
	UPROPERTY(EditDefaultsOnly)
		float InteractionRange = 20.f;
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemTable;
private:
	class AActor* LookAtActor;
	bool bActive = false;
	class UCWidget_DisplayMessage* DisplayMessage;

};
