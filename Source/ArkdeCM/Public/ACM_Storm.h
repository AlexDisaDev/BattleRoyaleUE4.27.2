// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_Storm.generated.h"

class UStaticMeshComponent;
class FProperty;

UCLASS()
class ARKDECM_API AACM_Storm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACM_Storm();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Static Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	float TimeElapsed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	FVector currentLocationScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	FVector nextLocationScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	float Lerpduration;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	float waitTime;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle TimerStormCloseMove;

	UFUNCTION(BlueprintCallable)
	void ChangeScale();

	UFUNCTION(BlueprintCallable)
	void ChangeLocation();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
