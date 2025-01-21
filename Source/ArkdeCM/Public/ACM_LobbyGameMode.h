// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArkdeCM/ArkdeCMGameMode.h"
#include "ACM_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_LobbyGameMode : public AArkdeCMGameMode
{
	GENERATED_BODY()
	
	AACM_LobbyGameMode();

public:
	virtual void PostLogin(APlayerController* Newplayer) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	FTimerHandle Timer;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Lobby")
	int MaxAmountOfPlayersToTravel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	float seconds;

	void CountdownTimer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CountDown Timer")
	bool timerOn;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	int CurrentPlayersOnLobby;
	
};
