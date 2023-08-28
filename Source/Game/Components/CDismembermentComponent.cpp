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
	ProcMesh->SetCollisionProfileName(TEXT("SlicedBody"));	// �ӽ�, WorldDynamic�� ������� �ʾƾ� ��������
}

void UCDismembermentComponent::OnSlice(ACharacter* SlicedCharacter, AActor* DamageCauser)
{
	if (SlicedCharacter == nullptr || ProcMesh == nullptr || bDoSlice == false)
		return;

	USkeletalMeshComponent* CharSkel = SlicedCharacter->GetMesh();
	CopySkeletalMeshToProcedural(CharSkel, 0, SlicedCharacter->GetCapsuleComponent(), ProcMesh);
	// 1. ��ġ, ȸ�� �ݿ�
	ProcMesh->SetWorldTransform(CharSkel->GetComponentTransform());
	// 2. ��Ƽ���� �ݿ�
	ProcMesh->SetMaterial(0, CharSkel->GetMaterial(0));
	// 3. ���⿡ ���� ����
	UShapeComponent* ShapeCauser = Cast<UShapeComponent>(DamageCauser->GetComponentByClass(UShapeComponent::StaticClass()));

	FVector start;
	FVector end;
	if (ShapeCauser)
	{
		end = start = ShapeCauser->GetComponentLocation();	// ���� ����
		FRotator rotater = ShapeCauser->GetComponentRotation();				// ���� ����

		// ���߿�
		FVector RotateVector = rotater.RotateVector(FVector(0, 0, 70.0f));
		
		start += RotateVector;
		end -= RotateVector;
	}

	TArray<FHitResult> hitResults;
	FHitResult UsedHitResult;
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(DamageCauser);
	ignoreActor.Add(DamageCauser->GetOwner());
	// ���� ũ���� Trace �����ؼ�, �ش� Trace�� ImpactPoint ���
	// Overlap �� ImpactPoint ���� �Ұ�

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
		UsedHitResult.ImpactPoint = DamageCauser->GetActorLocation();	// ���� �� ã����	
	else
		CLog::Log(UsedHitResult.Actor->GetName());
	// 4. ���� �ݿ�
	// �߷��� ProcMesh 2���� ����������
	//Slice

	UProceduralMeshComponent* outProcMesh = nullptr;
	// �ڸ��� ����
	FVector planeNormal = ShapeCauser->GetPhysicsAngularVelocityInDegrees().GetSafeNormal();

	// �ڸ��� �ܸ鵵
	UMaterialInstanceConstant* material =
		Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(),
			nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/Mat_Plane_Inst.Mat_Plane_Inst'")));

	// ��¥ �ڸ���
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
	 ProcMesh�� ����� CreateSection �� �ݸ�����, StaticMesh ����( ��, ���̷�Ż �޽ô� ����ƽ �޽��� �� �ݸ����� ����! )
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


	// �߹ٴ� ���ΰ� Z�� -1 �� �ƴϸ� �˴� ������� 140���� �ö�����, �Ƹ� HalfHeight �ι� �ؾ�������?
	// ��¶��, Ŀ���� Octahedron(�������) �浹ü Triangle ������ֱ�
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


//	ProcMeshComponent->AddCollisionConvexMesh(VerticesArray);	// �ʹ� �����ɸ�
}