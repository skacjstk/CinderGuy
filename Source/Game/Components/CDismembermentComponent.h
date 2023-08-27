// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDismembermentComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAME_API UCDismembermentComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UCDismembermentComponent();
	void OnSlice(class ACharacter* SlicedCharacter, class AActor* DamageCauser);
	//Skel, LODIndex, ProcMeshComp / 물체 velocity, 타점 Location
	bool GetDoSlice() { return bDoSlice; }

private:
	void CopySkeletalMeshToProcedural(class USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, class UCapsuleComponent* Capsule, class UProceduralMeshComponent* ProcMeshComponent);


public:
	UPROPERTY(EditAnyWhere)
		bool bDoSlice = true;
private:
	UPROPERTY(VisibleAnyWhere)
		class UProceduralMeshComponent* ProcMesh = nullptr;
};