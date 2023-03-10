#include "Characters/CPlayer.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CInventoryComponent.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerInput.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CWidget_PlayerHUD.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create SceneComponent
	CHelpers::CreateSceneComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateSceneComponent(this, &Camera, "Camera", SpringArm);

	// CreateActorComponent
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &Option, "Option");
	CHelpers::CreateActorComponent(this, &State, "State");
	CHelpers::CreateActorComponent(this, &Inventory, "Inventory");

	// Component Settings
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Get Subclass
	USkeletalMesh* meshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&meshAsset, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	CHelpers::GetClass<UUserWidget>(&DefaultHUDClass, "/Game/Widgets/WB_PlayerHUD");

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = false;

	// Movement Settings
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	//Create Dynamic 
	UMaterialInstanceConstant* bodyMaterial;
	UMaterialInstanceConstant* logoMaterial;
	CHelpers::GetAssetDynamic(&bodyMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic(&logoMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterial, nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);	// StateComponent??? ?????? ?????? ?????? ??????????????? ??????

	Action->SetUnarmedMode();
	Controller = Cast<APlayerController>(GetController());
	// "Action" ????????? ??? ??? ????????????
	if (!!Controller)	// CHelpers ??? ????????????
	{
		const TArray<FInputActionKeyMapping>& mapping = Controller->PlayerInput->GetKeysForAction("Action");
		for (const FInputActionKeyMapping map : mapping) {		
			if (map.ActionName.IsEqual("Action")) {
				ActionMapKey = map.Key;
			}
		}
	}

	// Player ?????? UI ??????


	if (GetController()->IsLocalPlayerController())
	{
		PlayerHUD = CreateWidget<UCWidget_PlayerHUD>(GetWorld(), DefaultHUDClass, "PlayerHUD");
		PlayerHUD->SetOwningPlayer( Cast<APlayerController>(GetController()) );
	}

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller->GetInputKeyTimeDown(FKey(ActionMapKey)) > 0.5f)
	{
		OnDoStrongAction();
	}
	
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Pressed, this, &ACPlayer::OnWalk);
	PlayerInputComponent->BindAction("Walk", EInputEvent::IE_Released, this, &ACPlayer::OffWalk);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction("MagicBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnMagicBall);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);

	PlayerInputComponent->BindAction("Katana", EInputEvent::IE_Pressed, this, &ACPlayer::OnKatana);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Repeat, this, &ACPlayer::OnDoStrongAction);	// ?????????
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, this, &ACPlayer::OffDoAction);	// ????????? ?????????
	
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);
	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnTestInventory);
}
FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACPlayer::OnMoveForward(float InAxis)
{
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->IsCanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());

}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += Option->GetZoomSpeed()* InAxis* GetWorld()->GetDeltaSeconds();
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, Option->GetZoomRange().X, Option->GetZoomRange().Y);
}

void ACPlayer::OnEvade() {

	CheckFalse(State->IsIdleMode());
	CheckFalse(Status->IsCanMove());

	if (InputComponent->GetAxisValue("MoveForward") < 0.f && FMath::IsNearlyZero(InputComponent->GetAxisValue("MoveRight")))
	{
		State->SetBackStepMode();
		return;
	}
	State->SetRollMode();
}

void ACPlayer::OnWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetWalkSpeed();
}

void ACPlayer::OffWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode()); 
	Action->SetFistMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetOneHandMode();
}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetTwoHandMode();
}

void ACPlayer::OnMagicBall()
{
	CheckFalse(State->IsIdleMode());
	Action->SetMagicBallMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());
	Action->SetWarpMode();
}
void ACPlayer::OnKatana()
{
	CheckFalse(State->IsIdleMode());
	Action->SetKatanaMode();
}
void ACPlayer::OnDoAction()
{
	Action->DoAction();
}
void ACPlayer::OnDoStrongAction()
{
	CheckTrue(State->IsStrongActionMode());
	CheckTrue(State->IsEndingStrongActionMode());	// StrongAction ?????? ????????? ?????? ???????????? ??????
	Action->DoStrongAction();
}
void ACPlayer::OffDoAction()
{
	CheckTrue(State->IsEndingStrongActionMode());
	Action->EndDoStrongActionWait();	// Ending ?????? ??????
}

void ACPlayer::OnAim()
{
	Action->DoOnAim();
}

void ACPlayer::OffAim()
{
	Action->DoOffAim();
}

void ACPlayer::OnTestInventory()
{
	PlayerHUD->DisplayPlayerMenu();
}

void ACPlayer::OnInteract()
{
	Inventory->Server_Interact_Implementation(nullptr);
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector end = start + GetVelocity().GetSafeNormal2D();

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, end));
	// ???????????? ????????? ????????? ?????? 
	Montages->PlayRoll();
}

void ACPlayer::Begin_BackStep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	Montages->PlayBackStep();
}

void ACPlayer::End_Roll()
{
	State->SetIdleMode();
	Status->SetMove();

	bool lookForward = Action->GetCurrent()->GetEquipment()->GetData().bPawnControl;
	bUseControllerRotationYaw = lookForward;
	GetCharacterMovement()->bOrientRotationToMovement = !lookForward;
}

void ACPlayer::End_BackStep()
{
	State->SetIdleMode();
	Status->SetMove();

	bool lookForward = Action->GetCurrent()->GetEquipment()->GetData().bPawnControl;
	bUseControllerRotationYaw = lookForward;
	GetCharacterMovement()->bOrientRotationToMovement = !lookForward;
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Causer = DamageCauser;
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	CLog::Print(DamageValue, -1, 1);

	Action->AbortByDamaged();

	Status->DecreaseHealth(this->DamageValue);
	if (Status->GetHealth() <= 0.f) {
		State->SetDeadMode();
		return this->DamageValue;
	}
	State->SetHittedMode();
	return this->DamageValue;
}

void ACPlayer::Hitted()
{
	Montages->PlayHitted();
//	Status->SetMove();	// Idle ?????? ??????????????? 
}

void ACPlayer::Dead()
{
	CheckFalse(State->IsDeadMode());
	Action->Dead();

	// Montage ??????: ??????????????? ?????? ???????????? 
	Montages->PlayDead();

	// End_Dead??? ???????????? ??????
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");
}

void ACPlayer::End_Dead()	// End_Dead??? ???????????? ??????
{
	Action->End_Dead();
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "Quit");	// ?????? ???????????? ????????????
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Roll:			Begin_Roll();			break;
	case EStateType::BackStep:		Begin_BackStep();		break;
	case EStateType::Hitted:		Hitted();				break;
	case EStateType::Dead:			Dead();					break;
	default:
		break;
	}
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

