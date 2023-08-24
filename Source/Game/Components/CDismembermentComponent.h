// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "CDismembermentComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UCDismembermentComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()
	
public:
	void SliceMesh(class USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, UProceduralMeshComponent* ProcMeshComponent);
	//Skel, LODIndex, ProcMeshComp, 뭐가 필요할까.

private:
	void CopySkeletalMeshToProcedural(class USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, UProceduralMeshComponent* ProcMeshComponent);


};
