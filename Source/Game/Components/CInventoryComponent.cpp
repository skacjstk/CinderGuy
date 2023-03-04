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
#include "Characters/CPlayer.h"
#include "Widgets/CWidget_DisplayMessage.h"

UCInventoryComponent::UCInventoryComponent()
{
	// 플레이어가 아닐 경우 Tick 꺼버리기 
	if (!!Cast<ACPlayer>(GetOwner()))
	{
		IsPlayer = true;
		PrimaryComponentTick.bCanEverTick = true;
	}
	else
	{
		IsPlayer = false;
		PrimaryComponentTick.bCanEverTick = false;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> defaultTable(TEXT("/Game/Inventory/DT_ItemData"));
	if (defaultTable.Succeeded())
		ItemTable = defaultTable.Object;

	ConstructorHelpers::FClassFinder<UCWidget_DisplayMessage> defaultDisplay(TEXT("/Game/Widgets/InventoryUI/WB_DisplayMessage"));
	if (defaultDisplay.Succeeded())	
		DisplayMessage = CreateWidget<UCWidget_DisplayMessage>(GetWorld(), defaultDisplay.Class);	
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
	Content.SetNumZeroed(InventorySize);// 인벤토리 사이즈 초기화
	if (!!DisplayMessage && IsPlayer)
	{
		DisplayMessage->SetOwningPlayer(Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)));
		DisplayMessage->AddToViewport();
	}
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
	if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, 15, ETraceTypeQuery::TraceTypeQuery3, false, ignoreActor, EDrawDebugTrace::None, hitResult, true))
	{
		if (LookAtActor != hitResult.GetActor())
		{
			LookAtActor = hitResult.GetActor();	// 대상이 같지 않으면 새로 삽입 후 실행
			
			bool bImpl = LookAtActor->Implements<UIInteract>();
			if (bImpl)
			{				
				IIInteract* interfaceActor = Cast<IIInteract>(LookAtActor);
				if (!!interfaceActor)
				{
					FText message;
					message = interfaceActor->Execute_LookAt(LookAtActor, LookAtActor);	// Implementation 사용시 바로 그 함수가 호출됨.
				
						ACItemBase* item = Cast<ACItemBase>(LookAtActor);
						int32 itemQuantity = 0;
						if (!!item)	// 아이템이 있으면 수량 받아오기, 아니라면 그냥 0
						{
							itemQuantity = item->GetItemData()->Quantity;
						}					
					if(!!DisplayMessage)
						DisplayMessage->ShowMessage(message, itemQuantity);
				}
			}
			// 객체가 블루프린트 단에서 Interface가 설치되면 작동하지 않음 (Cpp 이라면 컴파일 타임에 이미 들어있어야 하는 듯: CItem 만들때 상속하여 해결하기)
			// https://forums.unrealengine.com/t/c-uinterface-cast-failing-and-other-problems/95673/2
		}	
	}
	else
	{
		LookAtActor = nullptr;
		if(!!DisplayMessage)
			DisplayMessage->HiddenMessage();
	}
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
	MC_Update();
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
	FItem* item = ItemTable->FindRow<FItem>(InItemID, "");
	if(!!item)
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

bool UCInventoryComponent::InteractWith_Implementation(class ACharacter* playerCharacter)
{
	Server_Interact_Implementation(nullptr);
	return true;
}

void UCInventoryComponent::Server_Interact_Implementation(class AActor* Target)
{
	// Target 넣어놓긴 했는데 쓸일이 있나?
	if (!!LookAtActor)
	{	
		UCItemDataComponent* item = CHelpers::GetComponent<UCItemDataComponent>(LookAtActor);		
		if (!!item)
		{
			item->Execute_InteractWith(item, Cast<ACharacter>(GetOwner()));
		}
		else
		{	// Container 일 때의 기능 추가
			ACPlayer* player = Cast<ACPlayer>(GetOwner());
			if (player == nullptr) return;
			LookAtActor->SetOwner(player);// Todo: 만약 HUD를 Controller에 넣을 경우 얘도 바뀌어야 한다.
			OnLocalInteract(LookAtActor, player);
		}
		// 추가: Container 일 때에 대한 기능 추가
	}
}

void UCInventoryComponent::OnLocalInteract_Implementation(AActor* Target, AActor* Interactor)
{
	IIInteract* interact = Cast<IIInteract>(Target);
	if (!!interact)
		interact->Execute_InteractWith(Target, Cast<ACharacter>(Interactor));
	else
		CLog::Print("LoaclInteract Fail: you need Check Target's is work only cpp interface Implemet scope");
}

void UCInventoryComponent::DEBUG_PrintContents()
{
	for (FSlot& slot : Content)
	{
		CLog::Print(slot.ItemID.ToString() + ": "+ FString::FromInt(slot.Quantity));
	}
}

void UCInventoryComponent::MC_Update_Implementation()
{
	if (OnInventoryUpdated.IsBound())
		OnInventoryUpdated.Broadcast();
}

void UCInventoryComponent::RemoveFromInventory(int32 InSourceIndex, bool IsRemoveWhole, bool IsConsumed)
{
	FName selectedItem = Content[InSourceIndex].ItemID;
	int32 selectedQuantity = Content[InSourceIndex].Quantity;

	// 하나만 버리기 버리기
	if (!IsRemoveWhole || (selectedQuantity == 1))
	{
		Content[InSourceIndex].Quantity -= 1;	// 1 깎기
		if (!IsConsumed)
		{
			Server_DropItem(selectedItem, 1);	// 아이템 1개 떨구기
		}
	}
	else // 전체 버리기
	{	// Slot 없애버리기
		Content[InSourceIndex].ItemID = "";
		Content[InSourceIndex].Quantity = 0;
		if (!IsConsumed)	// 아이템을 사용해서 없애버린게 아니라면
		{
			Server_DropItem(selectedItem, selectedQuantity);
		}
	}
	MC_Update();
}

void UCInventoryComponent::Server_RemoveItem_Implementation(int32 InSourceIndex, bool IsRemoveWhole, bool IsConsumed)
{
	RemoveFromInventory(InSourceIndex, IsRemoveWhole, IsConsumed);
}

void UCInventoryComponent::Server_DropItem_Implementation(FName InItemID, int32 InQuantity)
{
	FItem * item = ItemTable->FindRow<FItem>(InItemID, "");	// ContextString은 에러 메시지
	if (item == nullptr) return;

	FTransform transform;

	FHitResult hitResult;
	TArray<AActor*> ignoreActor;
	ignoreActor.Add(GetOwner());
	FVector start;	// 1인칭 기본에 있던 거
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FRotator CamRot;
	controller->GetPlayerViewPoint(start, CamRot);
	FVector ShootDir = CamRot.Vector();

	start = start + ShootDir * ((GetOwner()->GetActorLocation() - start) | ShootDir);
	FVector end = start + ShootDir * 500;
		
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignoreActor, EDrawDebugTrace::None, hitResult, true))
	{
		transform.SetLocation(hitResult.Location);
	}
	else
	{
		transform.SetLocation(GetOwner()->GetActorLocation());
	}
	// 그냥 갯수를 주는 걸로 변경

	ACItemBase* itemActor = GetWorld()->SpawnActor<ACItemBase>(item->ItemClass, transform);	// 재대로 생성이 될까?	
	itemActor->GetItemData()->Quantity = InQuantity;
	// Todo: 여러개 소환 X, 그냥 n개짜리 소환 ㄱ
}

void UCInventoryComponent::Server_ConsumeItem_Implementation(FName InItemID)
{
	FItem* item = ItemTable->FindRow<FItem>(InItemID, "");
	if (item == nullptr) return;
	if (item->ItemEffectClass == nullptr) return;
	FTransform transform;

	ACItemEffectBase* itemEffectActor = GetWorld()->SpawnActorDeferred<ACItemEffectBase>(item->ItemEffectClass, transform, GetOwner());
	transform.SetLocation(GetOwner()->GetActorLocation());

	UGameplayStatics::FinishSpawningActor(itemEffectActor, transform);	// 여기가 진짜위치
}

void UCInventoryComponent::ConsumeItem(int32 index)
{
	FName& id = Content[index].ItemID;
	int32& quantity = Content[index].Quantity;	
	Server_ConsumeItem(id);
	Server_RemoveItem(index, false, true);	// 아이템 소모함.
}

void UCInventoryComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCInventoryComponent, Content);
	DOREPLIFETIME(UCInventoryComponent, OnInventoryUpdated);	// 델리게이트도 리플리케이트 등록이 되겠지?
}

void UCInventoryComponent::ToggleInventroy()
{
	bActive = !bActive;
	// Client 만 
}
// Drop에서 호출해서 DestComp는 필요가없다. 아이템을 교체하는 함수
void UCInventoryComponent::TransferSlots(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex)
{
	if (InSourceIndex < 0) return;	// Index 검사
	FSlot sourceSlot = InSourceInventory->Content[InSourceIndex];	// 기존 아이템

	// 같으면 아이템 합치기, 다르면 Swap
	// 아이템이 같으면 합치기, 최대수량을 파악해야 함.
	if (sourceSlot.ItemID == this->Content[InDestinationIndex].ItemID)
	{
		int32 maxQuantity = GetMaxStackSize(sourceSlot.ItemID);
		int32 cur = (Content[InDestinationIndex].Quantity + sourceSlot.Quantity) - maxQuantity;
		int remainQuantity = FMath::Clamp(cur, (int32)0 , maxQuantity);
		if (remainQuantity > 0)	// 0보다 크다는 건, 잔여량이 있다는 뜻
		{
			InSourceInventory->Content[InSourceIndex].Quantity = remainQuantity;
			this->Content[InDestinationIndex].Quantity = maxQuantity;
		}
		else // 잔여량이 없으면, Dest 쪽에 합병해버리기
		{
			this->Content[InDestinationIndex].Quantity += InSourceInventory->Content[InSourceIndex].Quantity;
			InSourceInventory->Content[InSourceIndex].Quantity = 0;
		}		
	}
	else  // 아이템이 다르면 Swap
	{		
		InSourceInventory->Content[InSourceIndex] = this->Content[InDestinationIndex];
		this->Content[InDestinationIndex] = sourceSlot;
	}
	// Todo: Source, Destination(나 자신), 의 인벤토리의 소유자가 Attachment 일 때, 그리고 아이템 Type이 Rune 일때, Equip, UnEquip 을 호출해준다. 
	MC_Update();
	InSourceInventory->MC_Update();	// 서로의 인벤토리 업데이트
}

void UCInventoryComponent::Server_TransferSlots_Implementation(int32 InSourceIndex, UCInventoryComponent* InSourceInventory, int32 InDestinationIndex)
{
	TransferSlots(InSourceIndex, InSourceInventory, InDestinationIndex);

}