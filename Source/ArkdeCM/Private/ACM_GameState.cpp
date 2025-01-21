// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_GameState.h"
#include "ACM_PlayerController.h"
#include "ACM_PlayerState.h"

AACM_GameState::AACM_GameState()
{
	seconds = 3.f;
}

void AACM_GameState::CheckWinCondition()
{
	int AlivePlayerCounter = 0;
	//AACM_PlayerController* GPlayerController = nullptr;
	AACM_PlayerController* WinningPlayerController = nullptr;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AACM_PlayerController* PlayerController = Cast<AACM_PlayerController>(It->Get());
		if (IsValid(PlayerController))
		{
			AACM_PlayerState* PlayerState = PlayerController->GetPlayerState<AACM_PlayerState>();
			if (IsValid(PlayerState))
			{
				if (PlayerState->IsAlive())
				{
					AlivePlayerCounter++;
					if (AlivePlayerCounter > 1)
					{
						return;
					}
					else
					{
						WinningPlayerController = PlayerController;
					}
				}
			}
		}
		
	}
	WinningPlayerController->GameConclussion(true);
	/*if (WinningPlayerController->bGameConclussion == true)
	{
		GetWorldTimerManager().SetTimer(Timer2, this, &AACM_GameState::CountdownTimer, 1.f, true, 0.f);
	}*/
	
}

//void AACM_GameState::CountdownTimer()
//{
//	
//	seconds--;
//
//	if (seconds <= 0)
//	{
//		
//		playerControllerGlobal->ClientTravel(FString(TEXT("LevelButtons")), ETravelType::TRAVEL_Absolute);
//		
//
//	}
//}



