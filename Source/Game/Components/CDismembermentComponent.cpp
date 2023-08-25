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

	// 1. ��ġ, ȸ�� �ݿ�
	ProcMesh->SetWorldTransform(CharSkel->GetComponentTransform());
	// 2. ��Ƽ���� �ݿ�
	ProcMesh->SetMaterial(0, CharSkel->GetMaterial(0));
		
	// 3. ���⿡ ���� ����
	DamageCauser->GetVelocity();
	// �����ϱ�: HitResult �� �浹������ �����;� ��.


	// 4. ���� �ݿ�(����)
	// 4-1. ���̷�Ż �޽� 2���� �غ��س���, ���� �и��� ProcMesh �� �����͸� �Ѱ��ٱ�??
	// 4-2. �ƴϸ� �߷��� ProcMesh 2���� �׳� ����������? (���� ������ �����ϸ鼭) ��Ʈ��, ��� ���ῡ�µ��� ���̷�Ż �޽ø� �ڸ��� ����ƽ�޽�2���� �ȴٰ� ����
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