#include "CEnemy.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Actions/CActionData.h"
#include "Actions/CThrow.h"
#include "GameFramework/CharacterMovementComponent.h"

ACEnemy::ACEnemy()
{
	//Create SceneComponent
	CHelpers::CreateSceneComponent(this, &NameWidget, "Name", GetMesh());
	CHelpers::CreateSceneComponent(this, &HealthWidget, "HealthWidget", GetMesh());
	//Create ActorComponent
	CHelpers::CreateActorComponent(this, &Action, "Action");
	CHelpers::CreateActorComponent(this, &Montages, "Montages");
	CHelpers::CreateActorComponent(this, &Status, "Status");
	CHelpers::CreateActorComponent(this, &State, "State");

	//Component Settings
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* meshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&meshAsset, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(meshAsset);

	TSubclassOf<UAnimInstance> animInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&animInstanceClass, "AnimBlueprint'/Game/Enemies/ABP_Enemy.ABP_Enemy_C'");
	GetMesh()->SetAnimInstanceClass(animInstanceClass);

	TSubclassOf<UCUserWidget_Name> nameWidgetClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameWidgetClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240,30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthWidgetClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthWidgetClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthWidgetClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(120, 20));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	//Movment Settings
	GetCharacterMovement()->MaxWalkSpeed = Status->GetRunSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACEnemy::BeginPlay()
{
	//Create Dynmaic Material
	UMaterialInstanceConstant* bodyMaterial;
	UMaterialInstanceConstant* logoMaterial;

	CHelpers::GetAssetDynamic(&bodyMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic(&logoMaterial, "MaterialInstanceConstant'/Game/Character/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyMaterial, nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(logoMaterial, nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	// StateType Changed Evnet
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	//Widget Property Settings

	CheckFalse(HasAuthority());
	NameWidget->InitWidget();
	UCUserWidget_Name* nameWidgetObject = Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject());
	if (!!nameWidgetObject)	
	{		
		nameWidgetObject->SetPawnName(GetName());
		nameWidgetObject->SetControllerName(GetController()->GetName());		// Controller Setting
	}
	
	HealthWidget->InitWidget();
	UCUserWidget_Health* healthWidgetObject = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(Status->GetHealth(), Status->GetMaxHealth());
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Idle:
		break;
	case EStateType::Roll:
		break;
	case EStateType::BackStep:
		break;
	case EStateType::Equip:
		break;
	case EStateType::Action:
		break;
	case EStateType::Hitted:	Hitted();	break;
	case EStateType::Dead:		Dead();		break;
	case EStateType::Max:
		break;
	default:
		break;
	}
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator , DamageCauser);
	Causer = DamageCauser;
	Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	CLog::Print(DamageValue, -1, 1);

	Status->DecreaseHealth(this->DamageValue);
	if (Status->GetHealth() <= 0.f) {
		State->SetDeadMode();
		return this->DamageValue;
	}
	State->SetHittedMode();
	return this->DamageValue;
}

void ACEnemy::ChangeColor(FLinearColor InColor)
{
	if (State->IsHittedMode())
	{
		LogoMaterial->SetVectorParameterValue("LogoLightColor", InColor);
		LogoMaterial->SetScalarParameterValue("IsHitted", 1);
		return;
	}
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::Hitted()	 
{
	// DecreaseHealth Widget
	UCUserWidget_Health* healthWidgetObject = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(Status->GetHealth(), Status->GetMaxHealth());

	// Play Hit Montage
	Montages->PlayHitted();

	// Launch HitBack
	FVector start = GetActorLocation();	// 나의 위치
	FVector target = Causer->GetActorLocation();	// 공격한 물체의 위치	(TakeDamage) 에서 받아왔음
	FVector direction =	(start - target);
	direction.Normalize();
	LaunchCharacter(direction * DamageValue * LaunchValue, true, false);

	ChangeColor(FLinearColor::Red * 100.f);
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreLogoColor", 1.f, false);
}
void ACEnemy::Dead() 
{
	CheckFalse(State->IsDeadMode());

	// Widget Visible false
	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	// All Weapon Collision Disable
	Action->Dead();

	// Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->GlobalAnimRateScale = 0.f;
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// AddForce(LaunchCharacter)
	FVector start = GetActorLocation();
	FVector target = Causer->GetActorLocation();
	FVector direction = start - target;
	direction.Normalize();

	// 투사체일 경우 DeadLaunch 를 낮추기 
	if (Causer->IsA<ACThrow>())
		DeadLaunchValue *= 0.075f;

	GetMesh()->AddForce(direction * DamageValue * DeadLaunchValue);
	
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACEnemy::End_Dead()
{
	// 다른애들은 직속 컴포넌트라 알아서 지워짐
	Action->End_Dead();
	Destroy();
}

void ACEnemy::RestoreLogoColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	LogoMaterial->SetVectorParameterValue("LogoLightColor", color);
	LogoMaterial->SetScalarParameterValue("IsHitted", 0);
}

