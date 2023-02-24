#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/Texture2D.h"
#include "Interfaces/IInteract.h"
#include "CInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		FName ItemID;
	UPROPERTY(EditDefaultsOnly)
		int32 Quantity;
};

USTRUCT(BlueprintType)
struct FItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
		FText Name;
	UPROPERTY(EditDefaultsOnly)
		FText Description;
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* Thumbnail;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditDefaultsOnly)
		int32 StckSize;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCInventoryComponent : public UActorComponent , public IIInteract
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UCInventoryComponent();

	void RemoveFromInventory();
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ToggleInventroy();
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// IInteract 상속
	virtual bool LookAt_Implementation(AActor* InActor, FText& OutMessage) override;
	virtual bool InteractWith_Implementation(class ACharacter* playerCharacter) override;

public:
	UFUNCTION(Reliable, Server)	// 서버에서 실행: 아니이거 전혀 다른데서 문제생기네
		void Server_Interact(class AActor* Target);
	void Server_Interact_Implementation(class AActor* Target);

public:
	void DEBUG_PrintContents();
private:
	UPROPERTY(EditDefaultsOnly)
		int32 InventorySize;
	UPROPERTY(EditDefaultsOnly)
		float InteractionRange = 20.f;
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemTable;
	UPROPERTY(Replicated)
		TArray<FSlot> Content;
private:
	class AActor* LookAtActor;
	bool bActive = false;

};
