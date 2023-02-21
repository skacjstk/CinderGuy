#include "CInventoryComponent.h"
#include "Global.h"
#include "Inventory/CInventoryBase.h"
#include "Inventory/CRuneInventory.h"
#include "Inventory/CInventory.h"
#include "Actions/CAttachment.h"
#include "GameFramework/Character.h"

UCInventoryComponent::UCInventoryComponent()
{
	// Player 또는 Attachment 마다 개별적으로 넣어주는 걸로 바뀜
//	CHelpers::GetClass<ACRuneInventory>(&RuneInventoryClass, "");
//	CHelpers::GetClass<ACInventory>(&InventoryClass, "");
}


void UCInventoryComponent::Open()
{
	Inventory->OpenInven();
}

void UCInventoryComponent::Close()
{
	Inventory->CloseInven();
}

// Called when the game starts
void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
	OwnerActor = GetOwner();
	FTransform transform = FTransform();
	// Attachment 일 경우, Inventory를 RuneInventory로 아니라면 CharInventory 로
	if (Cast<ACAttachment>(GetOwner()) != nullptr)
	{
		// ACRuneInventory를 생성하기
		Inventory = OwnerActor->GetWorld()->SpawnActorDeferred<ACRuneInventory>(RuneInventoryClass, transform, OwnerActor);

		Inventory->SetActorLabel("RuneInventory");
		UGameplayStatics::FinishSpawningActor(Inventory, transform);	
		
	}
	else if(Cast<ACharacter>(GetOwner()) != nullptr)
	{
		Inventory = OwnerActor->GetWorld()->SpawnActorDeferred<ACInventory>(InventoryClass, transform, OwnerActor);
		Inventory->SetActorLabel("CInventory");
		UGameplayStatics::FinishSpawningActor(Inventory, transform);	
	}
	else
	{
		CLog::Print("UCInventoryComponent__Not Attachment, Not Character, What is Owner?");
	}
}


