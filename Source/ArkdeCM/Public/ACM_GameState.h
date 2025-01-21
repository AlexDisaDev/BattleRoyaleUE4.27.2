// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ACM_GameState.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_GameState : public AGameStateBase
{
	GENERATED_BODY()

	AACM_GameState();

	
public:

	void CheckWinCondition();

	void CountdownTimer();

	void PlayerControllerTravel(APlayerController* player);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	bool timerOn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	float seconds;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	FTimerHandle Timer2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	APlayerController* playerControllerGlobal;
};
