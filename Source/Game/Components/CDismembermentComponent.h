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
	void OnSlice(class ACharacter* SlicedCharacter, class AActor* DamageCauser, FDamageEvent const& DamageEvent);
	//Skel, LODIndex, ProcMeshComp / 물체 velocity, 타점 Location

private:
	void CopySkeletalMeshToProcedural(class USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, class UProceduralMeshComponent* ProcMeshComponent);



private:
	UPROPERTY()
		class UProceduralMeshComponent* ProcMesh = nullptr;
};

/*
	//LineTrace
	FVector start = GetPawn()->GetActorLocation();
	FVector end = FVector(CursorLocation.X, CursorLocation.Y, start.Z);

	TArray<AActor*> ignores;
	ignores.Add(GetPawn());

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		1.f
	);
	if (hitResult.IsValidBlockingHit() == false) return;

	//Slice
	UProceduralMeshComponent* otherProcMesh = Cast<UProceduralMeshComponent>(hitResult.Component);
	if (otherProcMesh == nullptr) return;

	UProceduralMeshComponent* outProcMesh = nullptr;
	// 자르는 방향
	FVector direction = end - start;
	direction.Normalize();
	FVector planeNormal = GetPawn()->GetActorUpVector() ^ direction;

	// 자르는 단면도
	UMaterialInstanceConstant* material =
	Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(),
		nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/Mat_Plane_Inst.Mat_Plane_Inst'")));

	// 진짜 자르기
	UKismetProceduralMeshLibrary::SliceProceduralMesh
	(
		otherProcMesh,
		hitResult.Location,
		planeNormal,
		true,
		outProcMesh,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		material
	);

	outProcMesh->SetSimulatePhysics(true);
	outProcMesh->AddImpulse(planeNormal * -50000.f);
*/