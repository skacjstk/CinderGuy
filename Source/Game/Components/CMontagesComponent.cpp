#include "CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCMontagesComponent::UCMontagesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::BackStep);
}
void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());
	CheckNull(character);
	const FMontageData* data = Datas[(int8)InType];

	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRate, data->StartSection);
	}
}

