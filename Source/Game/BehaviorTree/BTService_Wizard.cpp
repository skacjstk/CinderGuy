#include "BTService_Wizard.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "CAIController.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStateComponent.h"

UBTService_Wizard::UBTService_Wizard()
{
	NodeName = "Wizard";
}

void UBTService_Wizard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(aiPawn);

	CheckTrue(state->IsDeadMode());

	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();
		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();
	if (target == nullptr)
	{
		behavior->SetWaitMode();
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	else // 대상의 상태가 Dead면 중지
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();	// Dead라면 Wait으로 
			return;
		}
	}

	// 플레이어 감지 시 
	controller->SetFocus(target);
	// 가까우면 Warp로 도망감, 멀면 Fireball
	float distane = aiPawn->GetDistanceTo(target);

	if (distane < controller->GetBehaviorRange())
	{
		behavior->SetAvoidMode();
		return;
	}

	if (distane < controller->GetSightRadius())
	{
		behavior->SetActionMode();
		return;
	}
}
