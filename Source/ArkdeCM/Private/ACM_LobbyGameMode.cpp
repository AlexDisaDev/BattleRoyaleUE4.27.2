// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_LobbyGameMode.h"
#include "ACM_GameInstance.h"

AACM_LobbyGameMode::AACM_LobbyGameMode()
{
	MaxAmountOfPlayersToTravel = 0;
	CurrentPlayersOnLobby = 0;
	timerOn = false;
	
}

void AACM_LobbyGameMode::PostLogin(APlayerController* Newplayer)
{
	Super::PostLogin(Newplayer);
	if (IsValid(Newplayer))
	{
		CurrentPlayersOnLobby++;

		if (CurrentPlayersOnLobby >= MaxAmountOfPlayersToTravel)
		{
			
			GetWorldTimerManager().SetTimer(Timer, this, &AACM_LobbyGameMode::CountdownTimer, 1.f, true, 1.f);
			
		}

	}
}

void AACM_LobbyGameMode::CountdownTimer()
{
	timerOn = true;
	seconds--;
	
	if (seconds <= 0)
	{
		UWorld* Gameworld = GetWorld();
		bUseSeamlessTravel = true;
		Gameworld->ServerTravel("/Game/BP/Maps/BattleRoyale?listen");
		seconds = 0.f;
		timerOn = false;
		GetWorldTimerManager().ClearTimer(Timer);
		
	}
}