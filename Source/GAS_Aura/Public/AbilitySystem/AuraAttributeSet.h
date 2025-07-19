// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAS_AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Mana,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxMana,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& Old) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& Old) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& Old) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& Old) const;
};
