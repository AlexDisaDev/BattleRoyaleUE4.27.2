// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_ActorAbilitySpawnOnBody.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ArkdeCM/ArkdeCMCharacter.h"

// Sets default values
AACM_ActorAbilitySpawnOnBody::AACM_ActorAbilitySpawnOnBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SphereComponentH = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponentH;

	ParticleSystemComponentH = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleSystemComponentH->SetupAttachment(RootComponent);

}

void AACM_ActorAbilitySpawnOnBody::Multicast_IgnoreActor_Implementation(AArkdeCMCharacter* ActorToIgnore)
{
	SphereComponentH->IgnoreActorWhenMoving(ActorToIgnore, true);
}

bool AACM_ActorAbilitySpawnOnBody::Multicast_IgnoreActor_Validate(AArkdeCMCharacter* ActorToIgnore)
{
	return true;
}

// Called when the game starts or when spawned
void AACM_ActorAbilitySpawnOnBody::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACM_ActorAbilitySpawnOnBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

