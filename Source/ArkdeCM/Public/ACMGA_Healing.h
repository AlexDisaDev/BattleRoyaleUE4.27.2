// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACM_GameplayAbility.h"
#include "ACMGA_Healing.generated.h"

class UAnimMontage;
class AACM_ActorAbilitySpawnOnBody;

/**
 * 
 */
UCLASS()
class ARKDECM_API UACMGA_Healing : public UACM_GameplayAbility
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* CastMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<AACM_ActorAbilitySpawnOnBody> BodySpawnPower;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag ProjectileSpawnTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProjectileRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName AbilitySocketName;


protected:

	UFUNCTION()
	void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
};
