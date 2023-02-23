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

	void AddToInventory();
	void RemoveFromInventory();
	void InteractionTrace();

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
	virtual bool InteractWith_Implementation() override;

public:
	UFUNCTION(Reliable, Server)	// 서버에서 실행: 아니이거 전혀 다른데서 문제생기네
		void Server_Interact(class AActor* Target);
	void Server_Interact_Implementation(class AActor* Target);

private:
	UPROPERTY(EditDefaultsOnly)
		int32 InventorySize;
	UPROPERTY(EditDefaultsOnly)
		float InteractionRange = 20.f;
	UPROPERTY(Replicated)
		TArray<FSlot> Content;
private:
	class AActor* LookAtActor;

	bool bActive = false;

};
