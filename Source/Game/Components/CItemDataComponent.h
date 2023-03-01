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
	virtual bool InteractWith_Implementation(class ACharacter* InPlayerCharacter) override;
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemData")
		FDataTableRowHandle ItemID;	// 포인터 쓸수없다.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ItemData")
		int32 Quantity = 1;

		
};
