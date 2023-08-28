#include "CDismembermentComponent.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/CLog.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DamageType/DamageTypeBase.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Kismet/KismetMathLibrary.h"

UCDismembermentComponent::UCDismembermentComponent()
{
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh");
	ProcMesh->SetVisibility(false);
	ProcMesh->bUseComplexAsSimpleCollision = false;
	ProcMesh->SetCollisionProfileName(TEXT("SlicedBody"));	// 임시, WorldDynamic을 블록하지 않아야 보기좋음
}

void UCDismembermentComponent::OnSlice(ACharacter* SlicedCharacter, AActor* DamageCauser)
{
	if (SlicedCharacter == nullptr || ProcMesh == nullptr || bDoSlice == false)
		return;

	USkeletalMeshComponent* CharSkel = SlicedCharacter->GetMesh();
	CopySkeletalMeshToProcedural(CharSkel, 0, SlicedCharacter->GetCapsuleComponent(), ProcMesh);
	// 1. 위치, 회전 반영
	ProcMesh->SetWorldTransform(CharSkel->GetComponentTransform());
	// 2. 머티리얼 반영
	ProcMesh->SetMaterial(0, CharSkel->GetMaterial(0));
	// 3. 방향에 맞춰 절단
	UShapeComponent* ShapeCauser = Cast<UShapeComponent>(DamageCauser->GetComponentByClass(UShapeComponent::StaticClass()));

	FVector start;
	FVector end;
	if (ShapeCauser)
	{
		end = start = ShapeCauser->GetComponentLocation();	// 월드 공간
		FRotator rotater = ShapeCauser->GetComponentRotation();				// 월드 공간

		// 나중에
		FVector RotateVector = rotater.RotateVector(FVector(0, 0, 70.0f));
		
		start += RotateVector;
		end -= RotateVector;
	}

	TArray<FHitResult> hitResults;
	FHitResult UsedHitResult;
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(DamageCauser);
	ignoreActor.Add(DamageCauser->GetOwner());
	// 같은 크기의 Trace 생성해서, 해당 Trace의 ImpactPoint 사용
	// Overlap 은 ImpactPoint 전달 불가

	if (UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, 16, ETraceTypeQuery::TraceTypeQuery4, false, 
		ignoreActor, EDrawDebugTrace::None, hitResults, false, FColor::Cyan, FColor::Green))
	{
		for (FHitResult& hitResult : hitResults)
		{
			if (hitResult.GetActor() == SlicedCharacter)
			{
				UsedHitResult = hitResult;
				break;
			}
		}
	}
	if (UsedHitResult.GetActor() == nullptr)	
		UsedHitResult.ImpactPoint = DamageCauser->GetActorLocation();	// 만약 못 찾으면	
	else
		CLog::Log(UsedHitResult.Actor->GetName());
	// 4. 물리 반영
	// 잘려진 ProcMesh 2개를 날려버리기
	//Slice

	UProceduralMeshComponent* outProcMesh = nullptr;
	// 자르는 방향
	FVector planeNormal = ShapeCauser->GetPhysicsAngularVelocityInDegrees().GetSafeNormal();

	// 자르는 단면도
	UMaterialInstanceConstant* material =
		Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(),
			nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/Mat_Plane_Inst.Mat_Plane_Inst'")));

	// 진짜 자르기
	UKismetProceduralMeshLibrary::SliceProceduralMesh
	(
		ProcMesh,
		UsedHitResult.ImpactPoint,
		planeNormal,
		true,
		outProcMesh,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		material
	);

	/*
	 ProcMesh가 만드는 CreateSection 에 콜리전은, StaticMesh 전용( 즉, 스켈레탈 메시는 스태틱 메시의 그 콜리전이 없다! )
	*/
	ProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProcMesh->SetVisibility(true);
	ProcMesh->SetSimulatePhysics(true);
	ProcMesh->AddImpulse(10000.0f * planeNormal);
	if (outProcMesh != nullptr)
	{
		outProcMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		outProcMesh->SetVisibility(true);
		outProcMesh->SetSimulatePhysics(true);
		outProcMesh->AddImpulse(-2000.f * planeNormal);
	}
}

void UCDismembermentComponent::CopySkeletalMeshToProcedural(USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, UCapsuleComponent* Capsule, UProceduralMeshComponent* ProcMeshComponent)
{
	if (ProcMeshComponent == nullptr)
		return;

	FSkeletalMeshRenderData* SkMeshRenderData = SkeletalMeshComponent->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[LODIndex];
	FSkinWeightVertexBuffer& SkinWeights = *SkeletalMeshComponent->GetSkinWeightBuffer(LODIndex);

	TArray<FVector> VerticesArray;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<int32> Tris;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	//get num vertices
	int32 NumSourceVertices = DataArray.RenderSections[0].NumVertices;

	for (int32 i = 0; i < NumSourceVertices; i++)
	{
		//get skinned vector positions
		FVector SkinnedVectorPos = USkeletalMeshComponent::GetSkinnedVertexPosition(SkeletalMeshComponent, i, DataArray, SkinWeights);
		VerticesArray.Add(SkinnedVectorPos);

		//Calc normals and tangents from the static version instead of the skeletal one
		FVector ZTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
		FVector XTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(i);

		//add normals from the static mesh version instead because using the skeletal one doesn't work right.
		Normals.Add(ZTangentStatic);

		//add tangents
		Tangents.Add(FProcMeshTangent(XTangentStatic, false));

		//get UVs
		FVector2D uvs = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
		UV.Add(uvs);

		//dummy vertex colors
		Colors.Add(FColor(0, 0, 0, 255));
	}


	//get index buffer
	FMultiSizeIndexContainerData indicesData;
	DataArray.MultiSizeIndexContainer.GetIndexBuffer(indicesData.Indices);

	//iterate over num indices and add traingles
	for (int32 i = 0; i < indicesData.Indices.Num(); i++)
	{
		uint32 a = 0;
		a = indicesData.Indices[i];
		Tris.Add(a);
	}

	//Create the procedural mesh
	ProcMeshComponent->CreateMeshSection(0, VerticesArray, Tris, Normals, UV, Colors, Tangents, false);


	// 발바닥 파인거 Z값 -1 대 아니면 죄다 양수였고 140까지 올라갔으니, 아마 HalfHeight 두배 해야할지도?
	// 어쨋든, 커스텀 Octahedron(수정모양) 충돌체 Triangle 만들어주기
	TArray<FVector> CollisionArray;

	float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float Radius = Capsule->GetScaledCapsuleRadius();

	CollisionArray.Add(FVector(0.0f,0.0f, 0.0f));	// 1
	CollisionArray.Add(FVector(-Radius,-Radius, HalfHeight));
	CollisionArray.Add(FVector(Radius,-Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, 0.0f));	// 1
	CollisionArray.Add(FVector(Radius, -Radius, HalfHeight));
	CollisionArray.Add(FVector(Radius, Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, 0.0f));	// 1
	CollisionArray.Add(FVector(Radius, Radius, HalfHeight));
	CollisionArray.Add(FVector(-Radius, Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, 0.0f));	// 1
	CollisionArray.Add(FVector(-Radius, Radius, HalfHeight));
	CollisionArray.Add(FVector(-Radius, -Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, HalfHeight * 2.0f));	// 1
	CollisionArray.Add(FVector(Radius, -Radius, HalfHeight));
	CollisionArray.Add(FVector(-Radius, -Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, HalfHeight * 2.0f));	// 1
	CollisionArray.Add(FVector(Radius, Radius, HalfHeight));
	CollisionArray.Add(FVector(Radius, -Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, HalfHeight * 2.0f));	// 1
	CollisionArray.Add(FVector(-Radius, Radius, HalfHeight));
	CollisionArray.Add(FVector(Radius, Radius, HalfHeight));

	CollisionArray.Add(FVector(0.0f, 0.0f, HalfHeight * 2.0f));	// 1
	CollisionArray.Add(FVector(-Radius, -Radius, HalfHeight));
	CollisionArray.Add(FVector(-Radius, Radius, HalfHeight));
	if (Capsule != nullptr)
		ProcMeshComponent->AddCollisionConvexMesh(CollisionArray);


//	ProcMeshComponent->AddCollisionConvexMesh(VerticesArray);	// 너무 오래걸림
}