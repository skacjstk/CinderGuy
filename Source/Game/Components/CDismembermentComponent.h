// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/CommonStructs.h"
#include "CDismembermentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAME_API UCDismembermentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UCDismembermentComponent();
	bool GetDoSlice() { return bDoSlice; }

protected:
	UFUNCTION()
		void OnStateTypePreChanged(EStateType PrevType, EStateType NewType, class AActor* DamageCauser = nullptr);
	virtual void BeginPlay() override;

private:
	void OnSlice(class AActor* DamageCauser);
	void CopySkeletalMeshToProcedural(class USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, class UCapsuleComponent* Capsule, class UProceduralMeshComponent* ProcMeshComponent);


public:
	UPROPERTY(EditAnyWhere)
		bool bDoSlice = true;
private:
	UPROPERTY(VisibleAnyWhere)
		class UProceduralMeshComponent* ProcMesh = nullptr;
};