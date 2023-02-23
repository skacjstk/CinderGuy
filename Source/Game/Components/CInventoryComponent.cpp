#include "CInventoryComponent.h"
#include "Net/UnrealNetwork.h"	
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Actor.h"
#include "Components/CItemDataComponent.h"
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
	FVector start;	// 1인칭 기본에 있던 거
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FRotator CamRot;
	controller->GetPlayerViewPoint(start, CamRot);
	FVector ShootDir = CamRot.Vector();

	start = start + ShootDir * ((GetOwner()->GetActorLocation() - start) | ShootDir);
	FVector end = start + ShootDir * InteractionRange;
	// 그래도 대략 맞는것 같다

	// 새로운 채널 3번
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 15, ETraceTypeQuery::TraceTypeQuery3, false, ignoreActor, EDrawDebugTrace::ForOneFrame, hitResult, true))
	{
		if (LookAtActor != hitResult.GetActor())
		{
			LookAtActor = hitResult.GetActor();	// 대상이 같지 않으면 새로 삽입 후 실행

			FText test = FText::FromString(LookAtActor->GetName());

			bool bImpl = LookAtActor->Implements<UIInteract>();
			if (bImpl)
			{	// 포럼 방식인 IIInteract::LookAt_Implementation() 은 아예 호출이 안된다.
				IIInteract* interact = Cast<IIInteract>(LookAtActor);
				if(!!interact)	// 이거 없어 
					interact->LookAt_Implementation(LookAtActor, test);
				// 액터의 인터페이스 재생
			}
			// 객체가 블루프린트 단에서 Interface가 설치되면 작동하지 않음 (Cpp 이라면 컴파일 타임에 이미 들어있어야 하는 듯: CItem 만들때 상속하여 해결하기)
			// https://forums.unrealengine.com/t/c-uinterface-cast-failing-and-other-problems/95673/2
		}	
	}
	else
	{
		LookAtActor = nullptr;
	}
}

bool UCInventoryComponent::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{
	CLog::Print("Interface Called");
	if (!!InActor)
		CLog::Print(InActor->GetName());
	else
		CLog::Print(*OutMessage.ToString());
	return true;
}

bool UCInventoryComponent::InteractWith_Implementation()
{
	return false;
}

void UCInventoryComponent::Server_Interact_Implementation(class AActor* Target)
{
	// Target 넣어놓긴 했는데 쓸일이 있나?
	if (!!LookAtActor)
	{		// GetComponent는 또 되네?? 
		UCItemDataComponent* item = CHelpers::GetComponent<UCItemDataComponent>(LookAtActor);		
		if (!!item)
			item->InteractWith_Implementation();
	}
}

void UCInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCInventoryComponent, Content);
}

void UCInventoryComponent::ToggleInventroy()
{
	bActive = !bActive;
	// Client 만 
}
