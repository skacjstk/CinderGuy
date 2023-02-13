#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

UCLASS()
class GAME_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	UCAim();
	void BeginPlay(class ACharacter* InOwnerCharacter);
	void Tick(float Deltatime);

	FORCEINLINE bool IsAvaliable() { return SpringArm != nullptr && Camera != nullptr; }// Aim 기능을 써도 되는 애인지 
	FORCEINLINE bool IsZooming() { return bZooming; }
	void On();
	void Off();
private:
	UFUNCTION()
		void Zooming(float Output);
private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

	bool bZooming;
private:
	class UCurveFloat* Curve;
	class ACHUD* Hud;
	FTimeline Timeline;

	FOnTimelineFloat TimelineFloat;
};
