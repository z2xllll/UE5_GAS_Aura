// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetMaxHealthAttribute())
	{
		
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		
	}
	else if (Attribute == GetHealthAttribute())
	{
		if (NewValue < 0)
		{
			//执行死亡逻辑
			UE_LOG(LogTemp, Error, TEXT("NewValue cannot be negative"));
		}
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		if (NewValue > 0 && NewValue < GetMaxMana())
		{
			//正常施法 
		}
		else if (NewValue < 0.f)
		{
			//停止施法
			UE_LOG(LogTemp, Error, TEXT("NewValue cannot be negative"));
			//法力值不变
			NewValue = GetMana();
		}
		else
		{
			NewValue = GetMaxMana();
		}
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
}


void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& Old) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, Old);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& Old) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, Old);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& Old) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, Old);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& Old) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, Old);
}
