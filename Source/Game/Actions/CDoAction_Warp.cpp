#include "CDoAction_Warp.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "AIController.h"
#include "CAIController.h"
#include "Actions/CAttachment.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	// 자손 찾기 
	for (AActor* actor : OwnerCharacter->Children)
	{	// Attachment 면서 Warp 일 경우, 내가 찾는 그게 맞음 
		if (actor->IsA<ACAttachment>() && actor->Tags.Contains("Warp"))
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(actor);
			SkelMesh = CHelpers::GetComponent<USkeletalMeshComponent>(actor);
			StaticMesh = CHelpers::GetComponent<UStaticMeshComponent>(actor);
			break;
		}
	}
}
void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped);
	CheckFalse(State->IsIdleMode());

	bool bPlayer = OwnerCharacter->IsA<ACPlayer>();
	if (bPlayer)
	{
		FRotator rotator;
		CheckFalse(GetCursorLocationAndRotation(Location, rotator));
	}
	else  // AI 일 경우
	{
		AAIController* controller = OwnerCharacter->GetController<AAIController>();
		UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);
		Location = behavior->GetWarpLocation();	// 이동위치를 Warp로 바꾸기

		Decal->SetVisibility(false);
		SkelMesh->SetVisibility(false);
		StaticMesh->SetVisibility(false);
	}


	State->SetActionMode();
	PlayAttackAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);	// 얘를 공격속도로 보는게 맞을까?
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;

	UGameplayStatics::SpawnEmitterAttached
	(
		Datas[0].Effect, 
		OwnerCharacter->GetMesh(),
		"",
		transform.GetLocation(),
		FRotator(transform.GetRotation()),
		transform.GetScale3D()
	);
}

void ACDoAction_Warp::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();

	Location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	OwnerCharacter->SetActorLocation(Location);
	Location = FVector::ZeroVector;
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(*bEquipped);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	FVector location;
	FRotator rotator;
	// 데칼 표시 및 회전 

	if (GetCursorLocationAndRotation(location, rotator))
	{
		FRotator ownerRotator = OwnerCharacter->GetController<APlayerController>()->GetControlRotation();
		ownerRotator.Yaw += 90.f;
		ownerRotator.Roll = 0.f;
		ownerRotator.Pitch = 0.f;

		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotator);

		SkelMesh->SetVisibility(true);
		SkelMesh->SetWorldLocation(location);
		SkelMesh->SetWorldRotation(ownerRotator);

		StaticMesh->SetVisibility(true);
		StaticMesh->SetWorldLocation(location);
		StaticMesh->SetWorldRotation(ownerRotator);
	}
	else
	{
		Decal->SetVisibility(false);
		SkelMesh->SetVisibility(false);
		StaticMesh->SetVisibility(false);
	}
}

bool ACDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// 커서 아래쪽 객체 확인
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1);
	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult))
	{
		OutLocation = hitResult.Location;
		OutRotator = hitResult.ImpactNormal.Rotation();
		return true;
	}
	
	return false;
}
