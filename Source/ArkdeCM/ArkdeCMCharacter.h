// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Engine/EngineTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "ArkdeCMCharacter.generated.h"


class UAbilitySystemComponent;
class UACM_AttributeSet;
class UACM_GameplayAbility;
class UGameplayEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceCharacterChange, float, PercentValue);


UCLASS(config=Game)
class AArkdeCMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AArkdeCMCharacter();
	//agregado
	virtual void BeginPlay() override;
	//agregado
	virtual void PossessedBy(AController* NewController) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	UPROPERTY(BlueprintAssignable)
	FOnResourceCharacterChange OnCharacterhealthDelegate;
	
	UFUNCTION()
	void OnCharacterhealthChanged(float health, float maxHealth);
	
	UFUNCTION(Client, Reliable)
	void Client_healthChanged(float health, float maxhealth);
	
	UPROPERTY(BlueprintAssignable)
	FOnResourceCharacterChange OnCharacterManaDelegate;
	
	UFUNCTION()
	void OnCharacterManaChanged(float mana, float maxmana);
	
	UFUNCTION(Client, Reliable)
	void Client_ManaChanged(float mana, float maxmana);


	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	bool IsInputbound;

	/*--- Gameplay abilities System Start*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UAbilitySystemComponent* AbilitySystemComponenet;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
	UACM_AttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
	TArray<TSubclassOf<UACM_GameplayAbility>> StartingAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
	TArray<TSubclassOf<UGameplayEffect>> StartingEffects;

	void SetupGasinputs();

	void SetupAbilities();

	void SetupEffects();

	bool AbilitiesGiven;

	bool EffectsGiven;

	/*---- Gameplay Ability System End ---*/
	bool IsDying;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DeathEffectClass;

	UFUNCTION(Server, Reliable)
	virtual void serverDie(AArkdeCMCharacter* killer);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnDeath();

	virtual void OnRep_PlayerState() override;

	

	

	
};

