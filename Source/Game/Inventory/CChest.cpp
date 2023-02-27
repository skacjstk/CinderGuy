#include "CChest.h"
#include "Global.h"
#include "Components/CInventoryComponent.h"

ACChest::ACChest()
{
	PrimaryActorTick.bCanEverTick = false;
	CHelpers::CreateActorComponent(this, &Inventory, "Inventory");
	Inventory->SetInventorySize(8);
}

// Called when the game starts or when spawned
void ACChest::BeginPlay()
{
	Super::BeginPlay();
	Inventory->Content.SetNumZeroed(Inventory->GetInventorySize());// 인벤토리 사이즈 초기화
}

// Called every frame
void ACChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

