#include "CAttachment.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CAttachmentStatusComponent.h"

// Sets default values
ACAttachment::ACAttachment()
{
	CHelpers::CreateActorComponent(this, &InventoryComponent, "RuneInventory");
	if (!!InventoryComponent)
		InventoryComponent->SetInventorySize(8);

	CHelpers::CreateSceneComponent(this, &Scene, "Scene");
}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	// 데이터 가져오는 것을 먼저 해야 함 BP의 호출순서 알지?

	GetComponents<UShapeComponent>(ShapeComponents);

	for (UShapeComponent* shape : ShapeComponents) {
		shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}
	OffCollisions();

	Super::BeginPlay();
	if (InventoryComponent == nullptr)
		CLog::Print("Null Inventory");
}

void ACAttachment::AttachTo(FName InSocketName)
{
	this->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::AttachToMesh(FName InSocketName, UStaticMeshComponent* AttachMesh)
{
	AttachMesh->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);	
}

void ACAttachment::AttachToCollision(USceneComponent* InComponent, FName InSocketName)
{
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);
	CheckTrue(OtherActor->GetClass() == OwnerCharacter->GetClass());	// 같은 타입끼리 치고박기 금지: 라곤 해도...


	if (OnAttachmentBeginOverlap.IsBound()) {
		ACharacter* otherCharacter = Cast<ACharacter>(OtherActor);
		CheckNull(otherCharacter);
		
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, otherCharacter);
	}
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OnAttachmentEndOverlap.IsBound()) {
		ACharacter* otherCharacter = Cast<ACharacter>(OtherActor);
		CheckNull(otherCharacter);

		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, otherCharacter);
	}
}

void ACAttachment::OnCollisions(FString InCollisionName)
{
	if (InCollisionName.Compare("None") == 0)
	{
		for (UShapeComponent* shape : ShapeComponents)
		{
			shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 적 밀려나는 효과랑 겹쳐서 잠시 QueryOnly 로 
		}	// 전체 모양 켜기
	}
	else
	{
		for (UShapeComponent* shape : ShapeComponents)
		{
			if(shape->GetName().Contains(InCollisionName))	// 콜리전 이름과 소켓이름을 일치시켜놨으니까 이렇게 되는거
				shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}

}
void ACAttachment::OffCollisions()
{
	for (UShapeComponent* shape : ShapeComponents)
	{
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

