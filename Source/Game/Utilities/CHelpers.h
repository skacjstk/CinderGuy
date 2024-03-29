#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"

#define CheckNull(p){ if(p == nullptr) return ; }
#define CheckNullResult(p, result){ if(p == nullptr) return result;}

#define CheckTrue(p){ if(p == true) return ; }
#define CheckTrueResult(p, result){ if(p == true) return result; }

#define CheckFalse(p){ if(p == false) return ; }
#define CheckFalseResult(p, result){ if(p == false) return result; }

class GAME_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutAsset ,FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"Asset Not Found");
		
		*OutAsset = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutAsset, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
		verifyf(!!obj, L"Asset Not Found(Dynamic)");

		*OutAsset = obj;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> pawnClass(*InPath);
		verifyf(pawnClass.Succeeded(), L"Class Not Found");

		*OutClass = pawnClass.Class;
	}

	template<typename T>
	static void GetClassDynamic(TSubclassOf<T>* OutClass, FString InPath)
	{
		UClass* obj = StaticCast<UClass*>(StaticLoadClass(T::StaticClass(), nullptr, *InPath));
		verifyf(!!obj, L"Class Not Found(Dynamic)");

		*OutClass = obj;
	}


	template<typename T>
	static void CreateSceneComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);
			return;
		}
		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}
	template<typename T>
	static T* GetComponent(AActor *InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
	template<typename T>	// 보통 FInputActionKeyMapping 
	static FKey GetKeyName(APlayerController* Controller, FName ActionName)
	{
		if (!!Controller)
		{
			const TArray<T>& mapping = Controller->PlayerInput->GetKeysForAction(ActionName);
			for (const T& map : mapping) {
				if (map.ActionName.IsEqual(ActionName)) {
					return map.Key;
				}
			}
		}
		return FKey();
	}

	static FString GetRoleText(ENetRole InRole)
	{
		switch (InRole)
		{
		case	ROLE_None:				return"None";
		case	ROLE_SimulatedProxy:	return"SimulatedProxy";
		case	ROLE_AutonomousProxy:	return"AutonomousProxy";
		case	ROLE_Authority:			return"Authority";
		case	ROLE_MAX:				return"MAX";
		default:						return "None";

		}
		return "None";
	}
};


/*
	static FKey GetKeyName(APlayerController* Controller, FName& ActionName)
	{
		if (!!Controller)
		{
			const TArray<T>& mapping = Controller->PlayerInput->GetKeysForAction(ActionName);
			for (const T map : mapping) {
				if (map.ActionName.IsEqual(ActionName)) {
					return map.Key;
				}
			}
		}
		return FKey();
	}
*/
