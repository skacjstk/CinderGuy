// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CommonStructs.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Normal,
	Rune,
	Max
};

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Roll, BackStep, Equip, Action, Guard, GuardBack, Parry, StrongAction, EndingStrongAction, Hitted, Dead, Max
};

USTRUCT(BlueprintType)
struct FItemDrop_CSV : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 DropTableID;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 ItemCode;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 Amount;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float DropChance;
};

USTRUCT(BlueprintType)
struct FDropItem : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 ItemCode;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 Amount;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float DropChance;
};

USTRUCT(BlueprintType)
struct FItemDropTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 DropTableID;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FDropItem> DropItemSet;
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase	// Editor���� �ν��ϰ� �ϱ� ����
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText Name;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		EItemType Type = EItemType::Normal;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FText Description;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UTexture2D* Thumbnail;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACItemBase> ItemClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int32 StckSize;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACItemEffectBase> ItemEffectClass;
};

UCLASS()
class GAME_API UCommonStructs : public UObject
{
	GENERATED_BODY()
	
};
