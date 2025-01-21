// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "AttributeSet.h"
#include "ACM_PlayerState.generated.h"


class UAbilitySystemComponent;
class UACM_AttributeSet;

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_PlayerState : public APlayerState, public  IAbilitySystemInterface
{
	GENERATED_BODY()
	
public: 
	AACM_PlayerState();

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UACM_AttributeSet* AttributeSet;
	

	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UACM_AttributeSet* GetAttributeSet() const;
	

	bool IsAlive() const;

	bool IsDead() const;

	float GetHealth() const;

	float GetMaxHealth() const;

	float  GetHealthRegen() const;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangedDelegateHandle;
	
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);

	/*---Win Condition  Start---*/


	int killCount;
	
	UFUNCTION(BlueprintCallable)
	void ScoreKill();


		/*---Win Condition Ends---*/
};
