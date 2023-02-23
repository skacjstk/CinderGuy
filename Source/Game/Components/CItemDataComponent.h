#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Interfaces/IInteract.h"
#include "CItemDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCItemDataComponent : public UActorComponent, public IIInteract
{
	GENERATED_BODY()

public:	
	UCItemDataComponent();

protected:
	virtual void BeginPlay() override;

public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// IInteract 상속
//	virtual bool LookAt_Implementation(AActor* InActor, FText& OutMessage) override;
	virtual bool InteractWith_Implementation() override;
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ItemData")
		FDataTableRowHandle ItemID;	// 포인터 쓸수있나?
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "ItemData")
		int32 Quantity = 1;

		
};