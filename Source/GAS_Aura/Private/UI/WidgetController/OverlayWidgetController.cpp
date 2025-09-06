// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//广播初始值
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	//Health
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	//Mana
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//绑定属性变化的回调,Health,MaxHealth,Mana,MaxMana
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()
		).AddUObject(this, &UOverlayWidgetController::HandleHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()
		).AddUObject(this, &UOverlayWidgetController::HandleMaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()
		).AddUObject(this, &UOverlayWidgetController::HandleManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()
		).AddUObject(this, &UOverlayWidgetController::HandleMaxManaChanged);

	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& Tags)
		{
			for (const FGameplayTag& Tag : Tags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("Message");
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* UIWidgetRow =	GetDataTableRow<FUIWidgetRow>(MessageWidgetDataTable,Tag);
					MessageWidgetRowDelegate.Broadcast(*UIWidgetRow);
				}
				
			}
		});
}

void UOverlayWidgetController::HandleHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::HandleMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

