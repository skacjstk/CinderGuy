#include "CEnemy.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CItemDropComponent.h"
#include "Components/CDismembermentComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Actions/CActionData.h"
#include "Actions/CThrow.h"
#include "GameFramework/CharacterMovementComponent.h"
// 테스트
#include "Components/CDamageEffectComponent.h"
#include "Actions/CActionObjectContainer.h"
#include "Actions/CDoAction.h"
#include "DamageType/KatanaParryDamageType.h"
#include "CDamageText.h"

#include "Net/UnrealNetwork.h"
#include "Utilities/CHelpers.h"

void ACEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACEnemy, DamageValue); 
}
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
	CHelpers::CreateActorComponent(this, &DamageEffect, "DamageEffect");
	CHelpers::CreateActorComponent(this, &ItemDrop, "ItemDrop");
	CHelpers::CreateActorComponent(this, &SliceBodyEffect, "SliceBodyEffect");

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

	//ItemDropComponent Settings
	ItemDrop->DropTable.DropTableID = 100;

	bReplicates = true;
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
	Status->OnUpdateHealth.AddDynamic(this, &ACEnemy::WidgetHeatlhUpdate);

	Super::BeginPlay();

	//Widget Property Settings

	NameWidget->InitWidget();
	UCUserWidget_Name* nameWidgetObject = Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject());
	if (!!nameWidgetObject)	
	{		
		nameWidgetObject->SetPawnName(GetName());
		// AI 컨트롤러는 서버에만 존재한다.
		nameWidgetObject->SetControllerName(GetController() != nullptr ? 
			GetController()->GetName() : CHelpers::GetRoleText(GetLocalRole())	);		// Controller Setting
	}
	
	HealthWidget->InitWidget();
	UCUserWidget_Health* healthWidgetObject = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(Status->GetHealth(), Status->GetMaxHealth());

//	Action->LateEquip();	// Attachment Replicate 끄면 절단효과 재현이 불가능
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType, AActor* DamageCauser)
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
	case EStateType::Hitted:	Hitted(DamageCauser);	break;
	case EStateType::Dead:		Dead(DamageCauser);		break;
	case EStateType::Max:
		break;
	default:
		break;
	}
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator , DamageCauser);

	if(!!EventInstigator)
		Attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	if (!!DamageEvent.DamageTypeClass && EventInstigator != nullptr)
	{
		IIDamageType* damageType = Cast<IIDamageType>(DamageEvent.DamageTypeClass->GetDefaultObject());
		if (!!damageType)
		{
			// 공격의 부가효과 재생
			damageType->Execute_DamageTrigger(DamageEvent.DamageTypeClass->GetDefaultObject(), DamageCauser, DamageEffect);
		}
	}
	if (!!DamageCauser)
	{
		IIDamageState* stateEffect = Cast<IIDamageState>(DamageCauser);
		if (!!stateEffect)
		{
			stateEffect->Execute_Damaged(DamageCauser);	// 이 DamageCauser 가 성공적으로 데미지를 주었음.
		}
		CLog::Print(DamageValue, -1, 1);
	}

	Status->DecreaseHealth(this->DamageValue);
	if (Status->GetHealth() <= 0.f) {
		// Need Event Call
		State->SetDeadMode(DamageCauser);
		return this->DamageValue;
	}
	State->SetHittedMode(DamageCauser);
	
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

void ACEnemy::WidgetHeatlhUpdate(float NewHealth)
{
	UCUserWidget_Health* healthWidgetObject = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	if (!!healthWidgetObject)
		healthWidgetObject->Update(NewHealth, Status->GetMaxHealth());

	SpawnDamageText();
}

void ACEnemy::Hitted(AActor* DamageCauser)
{
	// DecreaseHealth Widget ( 서버로 옮김 )
	// WidgetHeatlhUpdate();

	// Play Hit Montage
	Montages->PlayHitted();

	// Launch HitBack
	FVector start = GetActorLocation();	// 나의 위치
	FVector target = DamageCauser == nullptr ? FVector(0,0,0) : DamageCauser->GetActorLocation();	// 공격한 물체의 위치	(TakeDamage) 에서 받아왔음
	FVector direction =	(start - target);
	direction.Normalize();
	LaunchCharacter(direction * DamageValue * LaunchValue, true, false);

	// 데미지 띄우기
	// SpawnDamageText();	// 델리게이트로 이동
	// 바로 색 바꾸기
	ChangeColor(FLinearColor::Red * 100.f);
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreLogoColor", 1.f, false);
}
void ACEnemy::Dead(AActor* DamageCauser)
{
	CheckFalse(State->IsDeadMode());

	// Widget Visible false
	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);

	// All Weapon Collision Disable
//	SpawnDamageText();
	Action->Dead();

	// Ragdoll
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	if (SliceBodyEffect->GetDoSlice() == false)
	{
		GetMesh()->GlobalAnimRateScale = 0.f;
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		GetMesh()->SetVisibility(false);
		Action->End_Dead();
	}


	// AddForce(LaunchCharacter)
	FVector start = GetActorLocation();
	if (DamageCauser != nullptr)
	{
	//	CLog::Log(DamageCauser->GetName());
		FVector target = DamageCauser->GetActorLocation();
		FVector direction = start - target;
		direction.Normalize();

		// 투사체일 경우 DeadLaunch 를 낮추기 
		if (DamageCauser->IsA<ACThrow>())
			DeadLaunchValue *= 0.075f;
	}
	// Need Event
	if(HasAuthority())
		ItemDrop->DropItem(start);
//	GetMesh()->AddForce(direction * DamageValue * DeadLaunchValue);	// 일단 취소, 너무 멀리 날아가
	
	UKismetSystemLibrary::K2_SetTimer(this, "End_Dead", 5.f, false);
}

void ACEnemy::End_Dead()
{
	// 다른애들은 직속 컴포넌트라 알아서 지워짐
	Action->End_Dead();
	Destroy();
}

bool ACEnemy::CheckInvincible()
{
	return false;
}

bool ACEnemy::IsDead()
{
	return State->IsDeadMode();
}
void ACEnemy::RestoreLogoColor()
{
	if (IsPendingKill()) return;
	CheckTrue(State->IsDeadMode());

	return;	// TODO 얘는 대체 왜이러냐
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	LogoMaterial->SetVectorParameterValue("LogoLightColor", color);
	LogoMaterial->SetScalarParameterValue("IsHitted", 0);
}

void ACEnemy::SpawnDamageText()
{
	FTransform transform;
	ACDamageText* text = GetWorld()->SpawnActorDeferred<ACDamageText>(ACDamageText::StaticClass(), transform, this);
	transform.SetLocation(this->GetActorLocation());
	transform.SetRotation(FRotator(0, UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraRotation().Yaw, 0).Quaternion());
	UGameplayStatics::FinishSpawningActor(text, transform);
	text->SetDamage(DamageValue);
}

