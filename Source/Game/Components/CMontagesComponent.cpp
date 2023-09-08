#include "CMontagesComponent.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetWork.h"

void UCMontagesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UCMontagesComponent::UCMontagesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	if(GetOwnerRole() == ENetRole::ROLE_Authority)
		SetIsReplicatedByDefault(true);
}



// Called when the game starts
void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(DataTable);

	TArray<FMontageData*> datas;

	DataTable->GetAllRows<FMontageData>("", datas);	// 모두 담으려면 FString에 넣지말기

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type) {
				Datas[i] = data;
				continue;
			}
		}//end for
	}//end for	
}

void UCMontagesComponent::PlayRoll()
{
	Server_PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	Server_PlayAnimMontage(EStateType::BackStep);
}
void UCMontagesComponent::PlayHitted()
{
	Server_PlayAnimMontage(EStateType::Hitted);
}
void UCMontagesComponent::PlayDead()
{
	Server_PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::Server_PlayAnimMontage_Implementation(EStateType InType)
{
	MC_PlayAnimMontage(InType);
}

void UCMontagesComponent::MC_PlayAnimMontage_Implementation(EStateType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);
	const FMontageData* data = Datas[(int8)InType];

	if (!!data)
	{
		if (!!data->AnimMontage)
		{
			character->PlayAnimMontage(data->AnimMontage, data->PlayRate, data->StartSection);
		}

		UCStatusComponent* Status = CHelpers::GetComponent<UCStatusComponent>(character);
		if (!!Status)
			data->bCanMove ? Status->SetMove() : Status->SetStop();
	}
}
