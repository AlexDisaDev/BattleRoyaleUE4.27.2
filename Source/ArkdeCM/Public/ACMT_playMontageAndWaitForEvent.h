// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ACMT_playMontageAndWaitForEvent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FACMplayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

/**
 * 
 */
UCLASS()
class ARKDECM_API UACMT_playMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()
	
	public:
	UACMT_playMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	virtual void Activate();

	virtual void ExternalCancell();

	virtual FString GetDebugString() const;

	virtual void OnDestroy(bool bInOwnerFinished);

	UPROPERTY(BlueprintAssignable)
	FACMplayMontageAndWaitForEventDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
	FACMplayMontageAndWaitForEventDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FACMplayMontageAndWaitForEventDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FACMplayMontageAndWaitForEventDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
	FACMplayMontageAndWaitForEventDelegate OnEventReceived;



	UFUNCTION(BlueprintCallable, Category = "Ability|Task", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwingAbility", BlueprintInternalUseOnly = "TRUE"))
	static UACMT_playMontageAndWaitForEvent* playMontageAndWaitForEvent(UGameplayAbility* OwingAbility, FName taskInstanceName, UAnimMontage* Montageplay,
		FGameplayTagContainer EventTags, float Rate = 1.f, FName StartSection = NAME_None, bool bstopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f);

private:
	UPROPERTY()
	UAnimMontage* MontageToPlay;

	UPROPERTY()
	FGameplayTagContainer EventTags;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	bool StopPlayingMontage();

	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	void OnAbiltiyCancelled();

	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnGamePlayEvent(FGameplayTag EventTag, const FGameplayEventData* PayLoad) const;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;
};
