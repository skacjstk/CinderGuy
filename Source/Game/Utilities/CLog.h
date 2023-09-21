#pragma once

#include "CoreMinimal.h"

#define PrintLine() { CLog::Log(__FUNCTION__, __LINE__); }
#define FUNCTION_NAME (TEXT(" / Function Name : ") + FString(__FUNCTION__)+TEXT(" "))
#define CURRENT_CLASS (TEXT(" / Class Name : ") + FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) + TEXT(" "))
#define CURRENT_LINE  (TEXT(" / Line : ") + FString::FromInt(__LINE__) + TEXT(" "))
#define CURRENT_FUNCSIG (TEXT(" / Func Signature : ") +FString(__FUNCSIG__)+ TEXT(" "))
#define DEBUG_TEXT_POSITION (FUNCTION_NAME + CURRENT_LINE)
// Param0Àº FString À¸·Î
#define DEBUG_TEXT(Param0) (TEXT(Param0) + DEBUG_TEXT_POSITION)

class GAME_API CLog
{
public:
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor::Orange);
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor::Orange);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor::Orange);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor::Orange);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10.f, FColor InColor = FColor::Orange);

	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InObject);
	static void Log(const FString& InFuncName, int32 InLineNumber);
};
