// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_ActorAbilitySpawnOnBody.generated.h"


class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class AArkdeCMCharacter;

UCLASS()
class ARKDECM_API AACM_ActorAbilitySpawnOnBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACM_ActorAbilitySpawnOnBody();

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComponentH;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComponentH;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RangeH;

	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void Multicast_IgnoreActor(AArkdeCMCharacter* ActorToIgnore);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
