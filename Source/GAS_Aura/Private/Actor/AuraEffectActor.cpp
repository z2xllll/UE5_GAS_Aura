// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)return ;
	checkf(GameplayEffectClass, TEXT("Gameplay effect class must be a subclass"));

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && EffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		//store handle 
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (TSubclassOf<UGameplayEffect> &GameplayEffectClass:GameplayEffectClasses)
	{
		if (!IsValid(GameplayEffectClass))continue;
		//如果是瞬发效果,并且设置为在重叠时应用
		if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::Instant
			&& InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
		else if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::HasDuration
			&& DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
		else if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::Infinite
			&& InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
	}
	if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (TSubclassOf<UGameplayEffect> &GameplayEffectClass:GameplayEffectClasses)
	{
		if (!IsValid(GameplayEffectClass))continue;
		//如果是瞬发效果,并且设置为在重叠时应用
		if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::Instant
			&& InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
		else if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::HasDuration
			&& DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, GameplayEffectClass);
		}
		else if (GameplayEffectClass->GetDefaultObject<UGameplayEffect>()->DurationPolicy == EGameplayEffectDurationType::Infinite
			&& EffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
		{
			//remove infinite effect
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC))return;
			TArray<FActiveGameplayEffectHandle> HandlesToRemove;
			for (const TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& Pair : ActiveEffectHandles)
			{
				if (Pair.Value == TargetASC)
				{
					HandlesToRemove.Add(Pair.Key);
					TargetASC->RemoveActiveGameplayEffect(Pair.Key);
				}
			}
			for (const FActiveGameplayEffectHandle& Handle : HandlesToRemove)
			{
				ActiveEffectHandles.Remove(Handle);
			}
		}
	}
}



