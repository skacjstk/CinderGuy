#include "CItemBase.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CItemDataComponent.h"

ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	CHelpers::CreateSceneComponent(this, &ItemMesh, "ItemMesh", RootComponent);
	CHelpers::CreateActorComponent(this, &CItemData, "CItemData");
}

// Called when the game starts or when spawned
void ACItemBase::BeginPlay()
{
	Super::BeginPlay();	
}


FText ACItemBase::LookAt_Implementation(AActor* InActor)
{
	return FText();
}

bool ACItemBase::InteractWith_Implementation(ACharacter* playerCharacter)
{
	return false;
}

