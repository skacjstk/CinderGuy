#include "CItemBase.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

ACItemBase::ACItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	CHelpers::CreateSceneComponent(this, &ItemMesh, "ItemMesh", RootComponent);
	CHelpers::CreateActorComponent(this, &CItemData, "CItemData");
	bReplicates = true;
}

// Called when the game starts or when spawned
void ACItemBase::BeginPlay()
{
	Super::BeginPlay();	
}

bool ACItemBase::InteractWith_Implementation(ACharacter* playerCharacter)
{
	return false;
}

