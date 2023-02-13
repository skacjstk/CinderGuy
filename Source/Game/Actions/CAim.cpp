#include "CAim.h"
#include "Global.h"
#include "CHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

UCAim::UCAim()
{
	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Player/Curve_Aim.Curve_Aim'");
}

void UCAim::BeginPlay(ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;
	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	// Timeline 변수 설정
	TimelineFloat.BindUFunction(this, "Zooming");
	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200.f);

	// Get HUD
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD<ACHUD>();
	Hud = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();
}

void UCAim::Tick(float Deltatime)
{
	Timeline.TickTimeline(Deltatime);
}

void UCAim::On()
{
	CheckTrue(bZooming);
	bZooming = true;
	Hud->VisibleAim();
	CheckFalse(IsAvaliable());

	SpringArm->TargetArmLength = 100.f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;
	
	// 타임랑니 실행
	Timeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(bZooming);
	bZooming = false;
	Hud->HiddenAim();
	CheckFalse(IsAvaliable());

	SpringArm->TargetArmLength = 200.f;	// Todo: 옵션 넣기
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;
	
	Timeline.Reverse();
}

void UCAim::Zooming(float Output)
{
	Camera->FieldOfView = Output;	// Todo: DB로 ㄶ어보기 
}
