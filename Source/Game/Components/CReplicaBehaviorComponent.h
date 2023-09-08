// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CReplicaBehaviorComponent.generated.h"

/*
	 CAIController->BehaviorComponent ����  ����� �ൿ�� �޴� ������Ʈ
	 AIController �� Client�� �������� �ʾ�(���ٺҰ�) Controller �ȿ� �����ϴ� Behavior ����� ���� ���� ����.	
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
