#include "EnvQueryContext_Player.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* aiPawn = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());// 쿼리어를 실행하는 적 그 자체
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(aiPawn->GetController());
	
	if (behavior != nullptr)
	{
		if (!!behavior->GetTargetPlayer())	// 리턴을 스태틱으로 써줘야 함
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTargetPlayer());
	}
}