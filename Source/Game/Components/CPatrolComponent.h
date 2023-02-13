#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCPatrolComponent();

	FORCEINLINE bool IsValid() { return Path != nullptr; }

	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);	// 리터값은 갈 수 있는 지역인지 검사 
	void UpdateNext();	// BTTask 에서 호출함.
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;
	UPROPERTY(EditAnywhere)
		int32 Index = 0;
	UPROPERTY(EditAnywhere)
		bool bReverse;
	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 5.f;
};
