// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_Collectable.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AACM_Collectable::AACM_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	collisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Capsule Component"));
	RootComponent = collisionSphereComponent;

	collectableParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Collectable Particle System"));
	collectableParticleSystem->SetupAttachment(RootComponent);

	bReplicates = true;

}

// Called when the game starts or when spawned
void AACM_Collectable::BeginPlay()
{
	Super::BeginPlay();
	
}


