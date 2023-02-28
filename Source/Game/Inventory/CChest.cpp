#include "CChest.h"
#include "Global.h"
#include "Components/CInventoryComponent.h"

ACChest::ACChest()
{
	PrimaryActorTick.bCanEverTick = false;
	CHelpers::CreateActorComponent(this, &InventoryComponent, "Inventory");
	InventoryComponent->SetInventorySize(8);
}

// Called when the game starts or when spawned
void ACChest::BeginPlay()
{
	Super::BeginPlay();
	if(!!InventoryComponent)
		InventoryComponent->Content.SetNumZeroed(InventoryComponent->GetInventorySize());// 인벤토리 사이즈 초기화
}

// Called every frame
void ACChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

