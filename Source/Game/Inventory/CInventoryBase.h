#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInventoryBase.generated.h"

UCLASS()
class GAME_API ACInventoryBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ACInventoryBase();
	AActor* GetOwnerActor() { return OwnerActor; }

	virtual void EquipItem(int32 DragIndex, int32 DropIndex, class ACItemBase** prevItem, class ACItemBase** newItem) {};	// 2중 포인터는 순수가상함수가 될 수 없나보다.
	void SwapItem(class ACItemBase** itemA, class ACItemBase** itemB);	// 아이템을 참조로 넘기면 서로의 포인터를 바꿔줌

	virtual void GetSlots(TArray<class ACItemBase*> OutSlots) {}

	void OpenInven();
	void CloseInven();

protected:
	virtual void BeginPlay() override;
	AActor* OwnerActor;	// 이건, Player가 될 수도, Attachment가 될 수도 있음

	TSubclassOf<class UCUserWidget_Inventory> InventoryUIClass;
	class UCUserWidget_Inventory* InventoryUI;
};
