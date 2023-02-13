#include "BTTaskNode_Speed.h"
#include "Global.h"
#include "CAIController.h"
#include "Characters/CEnemy_AI.h"

UBTTaskNode_Speed::UBTTaskNode_Speed()
{
    NodeName = "Speed";
}

EBTNodeResult::Type UBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
 
    ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());   // 트리의 오너: 컨트롤러 
    ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(controller->GetPawn());
    UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(aiPawn);

    controller->GetPawn();
    status->SetSpeed(Type); // 스피드 값 설정

    return EBTNodeResult::Succeeded;
}
