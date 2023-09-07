#include "CAIController.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Components/CReplicaBehaviorComponent.h"
#include "Characters/CPlayer.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// AIController 가 없는 바깥쪽에 통신할 BehaviorComponent가 필요하다
//	CHelpers::CreateActorComponent<UCReplicaBehaviorComponent>(GetOwner(), &ReplicaBehavior, "ReplicaBehavior");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	// Sight Config 설정
	Sight->SightRadius = 600.f;
	Sight->LoseSightRadius = 800.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->SetMaxAge(2.f);
	Sight->DetectionByAffiliation.bDetectEnemies = true;	// 적 감지
	Sight->DetectionByAffiliation.bDetectFriendlies = false;	// 친구 감지
	Sight->DetectionByAffiliation.bDetectNeutrals = false;	// 중립 감지

	// Perception 에 Config 붙이고 Dominant 설정 
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(Sight->GetSenseImplementation());
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFalse(bDrawDebug);

	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= InitHeight;
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 50, FColor::Green, false, -1, 0, 0, 
		FVector::RightVector, FVector::ForwardVector);

	DrawDebugCircle(GetWorld(), center, BehaviorRange, 50, FColor::Red, false, -1, 0, 0,
		FVector::RightVector, FVector::ForwardVector);
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	// 트리에 있는 BB 변수를 Owner에 등록 
	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);	// 어떤 BB를 사용할지 정하는 함수

	// TeamID 설정
	SetGenericTeamId(OwnerEnemy->GetTeamID());
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// Blackboard 세팅해줌 // TODO: 클라에 존재하지 않음을 해결해야 함
	Behavior->SetBlackboard(Blackboard);
	// BT 실행
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();	// 모든 이벤트 언바인딩
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// 현재 감지가 된 애만 가져오기
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
//	Perception->GetPerceivedActors(nullptr, actors);
	ACPlayer* player = nullptr;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);
		if (!!player)
			break;
	}
	Blackboard->SetValueAsObject("Player", player);
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}