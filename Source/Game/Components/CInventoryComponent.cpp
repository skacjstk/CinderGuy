#include "CInventoryComponent.h"
#include "Net/UnrealNetwork.h"	
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Components/CItemDataComponent.h"
#include "Global.h"
#include "Engine/DataTable.h"

UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 

	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("DataTable'/Game/Inventory/DT_Item.DT_Item'"));
	if (defaultTable.Succeeded())
		ItemTable = defaultTable.Object;
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
	Content.SetNumZeroed(InventorySize);// 인벤토리 사이즈 초기화
}

void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractionTrace();
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
			{		

				// Todo: 상위 Cpp 클래스 상속 이후 사용
			//	IIInteract* interfaceActor = Cast<IIInteract>(LookAtActor);
			//	if(!!interfaceActor)
			//		interfaceActor->Execute_LookAt(LookAtActor, LookAtActor, test);
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

void UCInventoryComponent::RemoveFromInventory()
{
}

bool UCInventoryComponent::AddToInventory(FName InitemID, int32 InQuantity, int32& OutQuantityRemaining)
{
	int32 quantityRemaining = InQuantity;
	bool hasFailed = false;
	bool IsFound = false;
	while (!hasFailed && quantityRemaining > 0)
	{
		int32 index = FindSlot(InitemID, IsFound);
		if (IsFound)	// -1이 아니라면 찾은 것, 으로 해도 됨
		{
			AddToStack(index, 1);	// 기존 인벤토리에 아이템 추가 ( Todo: 1개씩 말고, 한번에 추가하고 넘치면 잔량 반환하도록 알고리즘 개선 )
			--quantityRemaining;
		}
		else  // 현재 Content 에 없는 itemID 이거나, 갯수가 꽉 찼을 때		
		{
			// 빈 칸 있다면 true 반환
			int32 emptyIndex;
			if (AnyEmptySlotsAvailable(emptyIndex))	// 현재 쓸데없이 두번 연산중
			{
				// 새로운 칸에 추가하기
				CreateNewStack(InitemID, 1, true, emptyIndex);
				--quantityRemaining;
			}
			else
			{
				hasFailed = true;	// 여기에 한번도 안걸리면 결국 성공 한거지 
			} //end if
		}//end if
	}//end while
	OutQuantityRemaining = quantityRemaining;
	return !hasFailed;	// 실패여부 역반환 반환값이 true면 성공임
}

int32 UCInventoryComponent::FindSlot(FName& InItemID, bool& OutFoundSlot)
{
	for (int32 i = 0; i < Content.Num(); ++i)
	{
		if (Content[i].ItemID == InItemID)
		{
			if (Content[i].Quantity < GetMaxStackSize(InItemID))	// 아이템이 존재하고, 남은 갯수가 
			{
				OutFoundSlot = true;
				return i;
			}
		}
	}
	// 찾기 실패
	OutFoundSlot = false;
	return -1;
}

int32 UCInventoryComponent::GetMaxStackSize(FName& InItemID)
{
	FItem* item = ItemTable->FindRow<FItem>(InItemID, "");	// ContextString은 에러 미시지
	if (!!item)
		return item->StckSize;
	else
		return -1;
}

void UCInventoryComponent::AddToStack(int32 InIndex, int32 InQuantity)
{
	Content[InIndex].Quantity += InQuantity;
}

bool UCInventoryComponent::AnyEmptySlotsAvailable(int32& OutEmptyIndex)
{
	for (int32 i = 0; i < Content.Num(); ++i)
	{
		if (Content[i].Quantity == 0)
		{
			OutEmptyIndex = i;
			return true;
		}
	}
	OutEmptyIndex = -1;
	return false;
}
/// <summary>
/// 
/// </summary>
/// <param name="InItemID">...</param>
/// <param name="InQuantity">...</param>
/// <param name="IsVerified">검증되었는지</param>
/// <param name="InVerifyIndex">검증되었다면, 어느 Index 인지, IsVerified = true 일 때만 유효하다.</param>
/// <returns></returns>
bool UCInventoryComponent::CreateNewStack(FName& InItemID, int32 InQuantity, bool IsVerified = false, int32 InVerifyIndex = -1)
{	
	int32 emptyIndex;
	if (IsVerified)
	{
		emptyIndex = InVerifyIndex;
		Content[emptyIndex].ItemID = InItemID;
		Content[emptyIndex].Quantity = InQuantity;
		return true;
	}
	else if(AnyEmptySlotsAvailable(emptyIndex))
	{
		Content[emptyIndex].ItemID = InItemID;
		Content[emptyIndex].Quantity = InQuantity;
		return true;
	}
	return false;
}


bool UCInventoryComponent::LookAt_Implementation(AActor* InActor, FText& OutMessage)
{	
	if (!!InActor)
		CLog::Print(InActor->GetName());
	else
		CLog::Print(*OutMessage.ToString());
	return true;
}

bool UCInventoryComponent::InteractWith_Implementation(class ACharacter* playerCharacter)
{
	return false;
}

void UCInventoryComponent::Server_Interact_Implementation(class AActor* Target)
{
	// Target 넣어놓긴 했는데 쓸일이 있나?
	if (!!LookAtActor)
	{		
		// GetComponent는 또 되네?? 
		UCItemDataComponent* item = CHelpers::GetComponent<UCItemDataComponent>(LookAtActor);		
		if (!!item)
		{
			item->Execute_InteractWith(item, Cast<ACharacter>(GetOwner()));
		}
	}
}

void UCInventoryComponent::DEBUG_PrintContents()
{
	for (FSlot& slot : Content)
	{
		CLog::Print(slot.ItemID.ToString() + ": "+ FString::FromInt(slot.Quantity));
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
