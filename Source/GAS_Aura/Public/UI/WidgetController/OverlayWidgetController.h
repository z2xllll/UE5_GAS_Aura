// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"


struct FOnAttributeChangeData;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate,FUIWidgetRow,MessageWidgetRow);
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class GAS_AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS | Attributes")
	FOnAttributeChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category = "GAS | Attributes")
	FOnAttributeChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category = "GAS | Attributes")
	FOnAttributeChanged OnManaChanged;

	UPROPERTY(BLueprintAssignable,Category = "GAS | Attributes")
	FOnAttributeChanged OnMaxManaChanged;

	UPROPERTY(BLueprintAssignable,Category = "GAS | Messages")
	FMessageWidgetRowDelegate MessageWidgetRowDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "WidgetData")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	void HandleHealthChanged(const FOnAttributeChangeData& Data) const;
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void HandleManaChanged(const FOnAttributeChangeData& Data) const;
	void HandleMaxManaChanged(const FOnAttributeChangeData& Data) const;

	template <typename T>
	T* GetDataTableRow(TObjectPtr<UDataTable> DataTable,const FGameplayTag& Tag);
	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRow(TObjectPtr<UDataTable> DataTable, const FGameplayTag& Tag)
{
	return DataTable ? DataTable->FindRow<T>(Tag.GetTagName(),TEXT("")) : nullptr;
}

