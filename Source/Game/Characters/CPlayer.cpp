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
#include "Actions/CActionObjectContainer.h"
#include "Actions/CDoAction.h"
#include "DamageType/KatanaParryDamageType.h"
#include "Interfaces/IDamageState.h"
#include "Components/CDamageEffectComponent.h"
#include "Net/UnrealNetwork.h"
#include "Utilities/CHelpers.h"

void ACPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACPlayer, TimelineFloat);
	DOREPLIFETIME(ACPlayer, GuardTimeline);
}

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

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
	CHelpers::CreateActorComponent(this, &DamageEffect, "DamageEffect");

	CHelpers::GetAsset<UCurveFloat>(&Curve, "CurveFloat'/Game/Player/Curve_Base.Curve_Base'");
	// Component Settings
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	// Get Subclass
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshAsset(TEXT("SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	GetMesh()->SetSkeletalMesh(meshAsset.Object);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 400.0f;
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

	// 생성자 시점에서 가져오기 불가능
	CHelpers::GetClassDynamic<UCWidget_PlayerHUD>(&DefaultHUDClass, TEXT("WidgetBlueprint'/Game/Widgets/WB_PlayerHUD.WB_PlayerHUD_C'"));

	//Create Dynamic 
	UMaterialInstanceConstant* bodyMaterial;
	UMaterialInstanceConstant* logoMaterial;
	CHelpers::GetAssetDynamic(&bodyMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic(&logoMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterial, nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);
	
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);	// StateComponent의 상태 변경 관련 델리게이트 등록

	Action->SetUnarmedMode();
	Controller = Cast<APlayerController>(GetController());
	// "Action" 매핑된 키 값 가져오기
	if (!!Controller)	// CHelpers 에 등록했음
	{
		const TArray<FInputActionKeyMapping>& mapping = Controller->PlayerInput->GetKeysForAction("Action");
		for (const FInputActionKeyMapping map : mapping) {		
			if (map.ActionName.IsEqual("Action")) {
				ActionMapKey = map.Key;
			}
		}
		const TArray<FInputActionKeyMapping>& aimMapping = Controller->PlayerInput->GetKeysForAction("Aim");
		for (const FInputActionKeyMapping map : aimMapping) {
			if (map.ActionName.IsEqual("Aim")) {
				AimMapKey = map.Key;	// 기본 우클릭 키
			}
		}
	}
	// Guard Timeline 매핑
	TimelineFloat.BindUFunction(this, "GuardAlpha");
	GuardTimeline.AddInterpFloat(Curve, TimelineFloat);
	GuardTimeline.SetPlayRate(1.0f);
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	DrawDebugString(GetWorld(), FVector(0, 0, 120), "Local: " + CHelpers::GetRoleText(GetLocalRole()), this, FColor::Black, DeltaTime, false, 1.2f);
	DrawDebugString(GetWorld(), FVector(0, 0, 180), "Remote: " + CHelpers::GetRoleText(GetRemoteRole()), this, FColor::Blue, DeltaTime, false, 1.2f);

	return;
	if (HasAuthority())
	{
		GuardTimeline.TickTimeline(DeltaTime);
	//	if (Controller->GetInputKeyTimeDown(FKey(ActionMapKey)) > 0.5f)
	//	{
	//		OnDoStrongAction();
	//	}
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
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Repeat, this, &ACPlayer::OnDoStrongAction);	// 강공격
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, this, &ACPlayer::OffDoAction);	// 강공격 해제용
	
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);

	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteract);
	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnTestInventory);
}
FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

UCWidget_PlayerHUD* ACPlayer::GetPlayerHUD()
{
	if (PlayerHUD == nullptr)
	{
		CreatePlayerHUD();
	}
	return PlayerHUD;
}

void ACPlayer::CreatePlayerHUD()
{
	PlayerHUD = CreateWidget<UCWidget_PlayerHUD>(GetWorld(), DefaultHUDClass, "PlayerHUD");
	PlayerHUD->SetOwningPlayer(Cast<APlayerController>(GetController()));
}

void ACPlayer::DestoryPlayerHUD()
{
	PlayerHUD->Destruct();	// 소멸까지
	PlayerHUD = nullptr;
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

	CheckFalse((State->IsIdleMode() || State->IsGuardMode()));
	CheckFalse(Status->IsCanMove());
	//InputComponent->GetAxisValue("MoveForward") < 0.f && FMath::IsNearlyZero(InputComponent->GetAxisValue("MoveRight")
	if (FMath::IsNearlyZero(InputComponent->GetAxisValue("MoveForward")) && FMath::IsNearlyZero(InputComponent->GetAxisValue("MoveRight")))
	{
		State->SetBackStepMode();
		return;
	}
	State->SetRollMode();
	// 반응 느린 문제 해결하려면, 얘는 키 Axis 와 Camera 바라보는 방향을가져다 줘야 함
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
	Action->Server_DoAction();
}
void ACPlayer::OnDoStrongAction()
{
	CheckTrue(State->IsStrongActionMode());
	CheckTrue(State->IsEndingStrongActionMode());	// StrongAction 관련 상태일 경우 수행하지 않음
	Action->DoStrongAction();
}
void ACPlayer::OffDoAction()
{
	CheckTrue(State->IsEndingStrongActionMode());
	Action->EndDoStrongActionWait();	// Ending 재생 대기
}

void ACPlayer::OnAim()
{
	Action->DoOnAim();
}

void ACPlayer::OffAim()
{
	Action->DoOffAim();
}

void ACPlayer::OnGuard()
{
	GuardTimeline.PlayFromStart();
}

void ACPlayer::OnTestInventory()
{
	// Player 기본 UI 생성
	if (GetController()->IsLocalPlayerController())
	{
		if (!!PlayerHUD)
		{
//			PlayerHUD->RemoveFromParent();
			DestoryPlayerHUD();
		}
		else
		{
			CreatePlayerHUD();
			PlayerHUD->DisplayPlayerMenu();
		}
	}
}

void ACPlayer::OnInteract()
{
	Inventory->Server_Interact_Implementation(Inventory->LookAtActor);
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector end = start + GetVelocity().GetSafeNormal2D();

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, end));
	// 바라보는 방향을 강제로 변경 
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

void ACPlayer::End_Parry()
{
	if (Controller->GetInputKeyTimeDown(FKey(AimMapKey)))	// 우클릭 누르고있으면, Guard로, 아니면 Idle 로
		State->SetGuardMode();		
	else
		State->SetIdleMode();

	Status->SetMove();
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (State->IsDeadMode()) return 0.0f;	// 데드모드면 아예 처리안해
	Causer = DamageCauser;
	if(!!EventInstigator)
		Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	if (State->IsGuardMode() || State->IsBlockMode() || State->IsParryMode())
	{
		if (State->GetGuardAlpha() + State->GetGuardFrame() >= 1.0f)	// 가드를 타이밍 맞게 눌렀다면
		{
			Action->DoParry();	// 패리 액션 (몽타주) 재생
			if (!!EventInstigator)
			{
				if (!!DamageCauser)
				{
					IIDamageState* damageState = Cast<IIDamageState>(DamageCauser);
					if (!!damageState)
					{
						damageState->Execute_Reflected(DamageCauser, Damage, GetController(), EventInstigator, DamageCauser);
					}
					else
					{
						// 현재 무기의 DamageTypeClass를 가져오기, 반격피해 주기
						TSubclassOf<UDamageType> damageType = Action->GetCurrent()->GetDoAction()->GetParryDamageType();
						if (!!damageType)
							EventInstigator->GetPawn()->TakeDamage(Damage, FDamageEvent(damageType), GetController(), this);
						// Todo: 반격 고유 피해 가져오기
					}
				}
			}
			return 0.0f;
		} // end if Parry
		else
		{
			Action->DoBlock();
			IIDamageState* test = Cast<IIDamageState>(DamageCauser);
			if (!!test)
			{
				test->Execute_Damaged(DamageCauser);
			}
			return 0.0f;
		} // end if Block
	}// end GuardMode

	// 피해를 입음
	if (!!DamageEvent.DamageTypeClass && EventInstigator != nullptr)
	{
		IIDamageType* damageType = Cast<IIDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		if (!!damageType)
		{
			// 공격의 부가효과 재생
			damageType->Execute_DamageTrigger(DamageEvent.DamageTypeClass->GetDefaultObject(), DamageEffect);
		}
	}
	CLog::Print(DamageValue, -1, 1);

	IIDamageState* DamageState = Cast<IIDamageState>(DamageCauser);
	if (!!DamageState)
	{
		DamageState->Execute_Damaged(DamageCauser);
	}

	Action->AbortByDamaged();

	Status->DecreaseHealth(this->DamageValue);
	if (Status->GetHealth() <= 0.f) {
		State->SetDeadMode();
		return this->DamageValue;
	}
	State->SetHittedMode();
	return this->DamageValue;
}

void ACPlayer::Hitted(AActor* DamageCauser)
{
	Montages->PlayHitted();
//	Status->SetMove();	// Idle 쪽에 넣어버렸음 
}

void ACPlayer::Dead(AActor* DamageCauser)
{
	CheckFalse(State->IsDeadMode());
	Action->Dead();

	// Montage 재생: 플레이어는 죽는 몽타주로 
	Montages->PlayDead();

	// End_Dead는 노티파이 재생
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");
}

void ACPlayer::End_Dead()	// End_Dead는 노티파이 재생
{
	Action->End_Dead();
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "Quit");	// 콘솔 명령어로 강제종료
}

bool ACPlayer::CheckInvincible()
{
	return true;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType, AActor* DamageCauser)
{
	switch (InNewType)
	{
	case EStateType::Roll:			Begin_Roll();			break;
	case EStateType::BackStep:		Begin_BackStep();		break;
	case EStateType::Hitted:		Hitted(DamageCauser);	break;
	case EStateType::Dead:			Dead();					break;
	case EStateType::Guard:			OnGuard();				break;
	default:
		break;
	}
}

void ACPlayer::GuardAlpha(float Output)
{
	State->SetGuardAlpha(Output);	// 가드 알파를 세팅
}

void ACPlayer::ChangeColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

