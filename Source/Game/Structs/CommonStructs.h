// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CommonStructs.generated.h"

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

UCLASS()
class GAME_API UCommonStructs : public UObject
{
	GENERATED_BODY()
	
};
