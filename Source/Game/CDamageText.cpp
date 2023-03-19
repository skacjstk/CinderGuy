#include "CDamageText.h"
#include "Global.h"
#include "Components/TextRenderComponent.h"

ACDamageText::ACDamageText()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateSceneComponent(this, &DamageText, "DamageText", RootComponent);
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Player/Curve_FloatingText.Curve_FloatingText'");
}

void ACDamageText::BeginPlay()
{
	Super::BeginPlay();
	
	TimelineFloat.BindUFunction(this, "FloatingAlpha");
	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(1.0f);
	EndEvent.BindUFunction(this, "DestroyThis");
	Timeline.SetTimelineFinishedFunc(EndEvent);
}

void ACDamageText::DestroyThis()
{
	Destroy();
}

void ACDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);
}

void ACDamageText::SetDamage(float damage)
{
	if(!!DamageText)
		DamageText->SetText(FString::SanitizeFloat(damage, 0));
	
	StartLocation = GetActorLocation();
	DamageText->AddRelativeRotation(FRotator(0,180,0).Quaternion());
	Timeline.PlayFromStart();
}

void ACDamageText::FloatingAlpha(float Output)
{
	SetActorLocation(FVector(StartLocation.X, StartLocation.Y, StartLocation.Z + Output + 80.f));
}