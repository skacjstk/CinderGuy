// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

// Todo: 여기부터
UCLASS()
class GAME_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	virtual void DoAction()				override;
	virtual void Begin_DoAction()		override;
	virtual void DoStrongAction()		override;
	virtual void EndDoStrongAction()	override;
	virtual void CheckEndDoStrongAction() override;		// 노티파이 호출
	virtual void EndDoStrongActionWait()	override;
	virtual void End_DoAction()			override;

	// 부모에서 UFUNCTION() 이면, 자식은 빠져도 됨.
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter)override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InCauser, class ACharacter* InOtherCharacter) override;

private:
	UFUNCTION()
		void RestoreGlobalTimeDilation();
public: //노티파이 호출
	FORCEINLINE void EnableCombo() { bCanCombo = true; }
	FORCEINLINE void DisableCombo() { bCanCombo = false; }
	FORCEINLINE void ClearHittedCharacters() { HittedCharacters.Empty(); }
	FORCEINLINE FString GetSpecificCollisionName() { return Datas[ComboCount].SpecificCollisionName; }

private:
	int32 ComboCount = 0;	// 평타 콤보
//	int32 SamshCount;	// 커스텀: 언젠가 스매시 만들 것comboCount * SmashCount 곱으로 스매시 데이터 접근 
	bool bCanCombo;	//콤보 허용구간 켜고 끄기
	bool bSucceed;	//콤보 성공여부
	bool IsStrongAction = false;	// 오직 HitStop 변수 파악용
	bool NextEndStrongAction = false;	// StrongAction의 End 파트를 재생할 여부를 결정할 변수
	TArray<class ACharacter*> HittedCharacters;	// 다단히트 방지용 타격대상 등록 

};
