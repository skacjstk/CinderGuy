#include "CDismembermentComponent.h"
#include "ProceduralMeshComponent.h"
#include "Utilities/CLog.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"

UCDismembermentComponent::UCDismembermentComponent()
{
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProcMesh");
	ProcMesh->SetVisibility(false);
	ProcMesh->SetSimulatePhysics(false);
}

void UCDismembermentComponent::OnSlice(ACharacter* SlicedCharacter, AActor* DamageCauser)
{
	if (SlicedCharacter == nullptr || ProcMesh == nullptr)
		return;
	USkeletalMeshComponent* CharSkel = SlicedCharacter->GetMesh();;

	ProcMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CopySkeletalMeshToProcedural(CharSkel, 0, ProcMesh);

	// 1. 위치, 회전 반영
	ProcMesh->SetWorldTransform(CharSkel->GetComponentTransform());
	// 2. 머티리얼 반영
	ProcMesh->SetMaterial(0, CharSkel->GetMaterial(0));
		
	// 3. 방향에 맞춰 절단
	DamageCauser->GetVelocity();
	// 내일하기: HitResult 에 충돌지점을 가져와야 함.


	// 4. 물리 반영(난항)
	// 4-1. 스켈레탈 메시 2개를 준비해놓고, 각각 분리된 ProcMesh 의 데이터를 넘겨줄까??
	// 4-2. 아니면 잘려진 ProcMesh 2개를 그냥 날려버릴까? (복잡 물리학 적용하면서) 힌트로, 상용 유료에셋들이 스켈레탈 메시를 자르면 스태틱메시2개가 된다고 했음
	ProcMesh->SetVisibility(true);	
	CharSkel->SetVisibility(false);
}

void UCDismembermentComponent::CopySkeletalMeshToProcedural(USkeletalMeshComponent* SkeletalMeshComponent, int32 LODIndex, UProceduralMeshComponent* ProcMeshComponent)
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
	ProcMeshComponent->CreateMeshSection(0, VerticesArray, Tris, Normals, UV, Colors, Tangents, true);
}