// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionObjectContainer.h"
#include "Components/CRuneInventoryComponent.h"
#include "Items/ItemRuneBase.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlp, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlp, class ACharacter*, InAttacker, class AActor*, InCauser, class ACharacter*, InOtherCharacter);

UCLASS()
class GAME_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACAttachment();

	FORCEINLINE class UCAttachmentStatusComponent* GetAttachmentStatusComponent() { return AttachmentStatus; }
	void SetAttachmentStatusComponent(class UCAttachmentStatusComponent* attachmentStatus) { this->AttachmentStatus = attachmentStatus; }	
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCInventoryComponent* GetAttachmentInventoryComponent() { return Cast<class UCInventoryComponent>(InventoryComponent); }

	// 룬 저장, 해제
	void SetAttachmentRune(int32 index, class AItemRuneBase* newRune) { AttachmentRunes[index] = newRune; }
	AItemRuneBase* GetAttachmentRune(int32 index) { return AttachmentRunes[index]; }
	void RemoveAttachmentRune(int32 index) {AttachmentRunes[index]->Destroy(); AttachmentRunes[index]= nullptr;	}
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnCollisions(FString InCollisionName = "None");	// 노티파이 함수
	void OffCollisions();

	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();				// CActionData 에서 바인딩
	UFUNCTION(BlueprintImplementableEvent)	// 우리 Equipment Delegate 가 이런 타입
		void OnUnequip();

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSocketName);
	UFUNCTION(BlueprintCallable)
		void AttachToMesh(FName InSocketName, UStaticMeshComponent* AttachMesh);
	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class USceneComponent* InComponent, FName InSocketName);
private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 필드

public:
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlp OnAttachmentBeginOverlap;
	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlp OnAttachmentEndOverlap;
	//AttachmentStatus
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AttachmentStatus")
		FAttachmentStatusData AttachmentStatusData;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;
protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadOnly)
		class UDataAsset* StatusAsset;
protected:
	TArray<class UShapeComponent*> ShapeComponents;	// 무기 충돌체 들
	class UCAttachmentStatusComponent* AttachmentStatus;	// 무기의 기본 피해량 등을 담은 클래스
	class UCRuneInventoryComponent* InventoryComponent;
	TArray<class AItemRuneBase*> AttachmentRunes;	// 장착된 룬들의 효과? 를 저장할 변수
};
