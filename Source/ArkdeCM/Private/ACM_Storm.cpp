// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_Storm.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AACM_Storm::AACM_Storm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	waitTime = 3.f;
	Lerpduration = 3.f;
	
}

// Called when the game starts or when spawned
void AACM_Storm::BeginPlay()
{
	Super::BeginPlay();
	
	currentLocationScale = GetActorLocation();
}

void AACM_Storm::ChangeScale()
{
	
}


void AACM_Storm::ChangeLocation()
{
	
}

//Called every frame
void AACM_Storm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (waitTime > 0)
	{
		waitTime -= DeltaTime;
		return;
	}

	if (TimeElapsed < Lerpduration)
	{

		this->SetActorLocation(FMath::Lerp(currentLocationScale, nextLocationScale, TimeElapsed / Lerpduration));
	}
}

