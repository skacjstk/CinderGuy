#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWidget_InventrorySlot.generated.h"

UCLASS()
class GAME_API UCWidget_InventrorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UCWidget_InventrorySlot(const FObjectInitializer& ObjectInitializer);

public:
	void InitItem(FName InItemID, int32 InQuantity, class UCInventoryComponent* InInventoryComponent, int32 InIndex);
	virtual bool Initialize() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FText GetDescription();

protected:
//	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;	// DragDrop 마우스이벤트
	virtual void NativePreConstruct() override;
	UFUNCTION(BlueprintCallable)
		void InitWidgets(class UButton* InButton, class  UImage* InImage, class UTextBlock* InText, class  USizeBox* InBox);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FKey GetActionEventKey(FName ActionName);
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (BindWidget))
	class UButton* ItemButton;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (BindWidget))
	class UImage* ItemImage;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (BindWidget))
		class UTextBlock* TextQuantity;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (BindWidget))
		class USizeBox* BoxQuantity;
protected: 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemValue", meta = (ExposeOnSpawn = true))
		FName ItemID = "None";
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemValue", meta = (ExposeOnSpawn = true))
		int32 Quantity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemValue", meta = (ExposeOnSpawn = true))
		int32 ContentIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
		class UCInventoryComponent* InventoryComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UDataTable* ItemTable;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UDataTable* RuneTable;
	TSubclassOf<class UCWidget_DragPreview> PreviewClass;

	UPROPERTY(BlueprintReadOnly)
		FText itemDescription;

};
