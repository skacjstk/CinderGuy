// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CReplicaBehaviorComponent.generated.h"

/*
	 CAIController->BehaviorComponent 에서  명령한 행동을 받는 컴포넌트
	 AIController 는 Client에 존재하지 않아(접근불가) Controller 안에 존재하는 Behavior 명령을 내릴 수가 없다.	
*/


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCReplicaBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCReplicaBehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
