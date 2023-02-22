#include "CInventoryComponent.h"
#include "Net/UnrealNetwork.h"	
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "Global.h"

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
	SetIsReplicated(true);
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
}

void UCInventoryComponent::AddToInventory()
{
}

void UCInventoryComponent::RemoveFromInventory()
{
}

void UCInventoryComponent::InteractionTrace()
{
	FHitResult hitResult;
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(GetOwner());
	APlayerCameraManager* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector start = camera->K2_GetActorLocation();	// 왜 그냥 GetActorLocation은 접근이 불가한가?
		// 그래도 대략 맞는것 같다
	FVector end = start + camera->GetActorForwardVector() * InteractionRange;

	// 새로운 채널 3번
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 15, ETraceTypeQuery::TraceTypeQuery3, false, ignoreActor, EDrawDebugTrace::None, hitResult, true))
	{
		LookAtActor = hitResult.GetActor();
		if (!!LookAtActor)
		{
			FText test = FText::FromString(LookAtActor->GetName());
			IIInteract* interfaceActor = Cast<IIInteract>(LookAtActor);	// 가져온 Actor에 Interact Casting이 안되는 이유
			// https://forums.unrealengine.com/t/c-uinterface-cast-failing-and-other-problems/95673/2
			// 객체가 블루프린트 단에서 Interface가 설치되면 작동하지 않음 (Cpp 이라면 컴파일 타임에 이미 들어있어야 하는 듯)
			if (!!interfaceActor)
			{
				interfaceActor->LookAt_Implementation(LookAtActor, test);
			}
			else if(interfaceActor == nullptr)
			{
				CLog::Print("Interface Nullptr");
			}
		}	
	}
	else
	{
		LookAtActor = nullptr;
	}
}

bool UCInventoryComponent::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	if (!!InActor)
		CLog::Print(InActor->GetName());
	else
		CLog::Print(*OutMessage.ToString());
	return true;
}

bool UCInventoryComponent::InteractWith_Implementation()
{
	return true;

}


void UCInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCInventoryComponent, Content);
}