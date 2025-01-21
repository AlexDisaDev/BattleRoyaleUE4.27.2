// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkdeCMGameMode.h"
#include "ArkdeCMCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "ACM_PlayerController.h"
#include "ACM_GameState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "ACM_Spawner.h"

AArkdeCMGameMode::AArkdeCMGameMode()
{
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP/Character/BP_ArkdeCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		
	}
}

void AArkdeCMGameMode::playerKilled(AController* VictimController)
{
	AACM_PlayerController* ArkdeVictimController = Cast<AACM_PlayerController>(VictimController);
	if (IsValid(ArkdeVictimController))
	{
		ArkdeVictimController->GameConclussion(false);
		playerAlives--;
		
	}


	AACM_GameState* WorldGameState = GetWorld()->GetGameState<AACM_GameState>();
	if (IsValid(WorldGameState))
	{
		WorldGameState->CheckWinCondition();
		if (playerAlives == 0)
		{
			BP_DestroyStorm();
		}
	}
}

void AArkdeCMGameMode::BeginPlay()
{
	Super::BeginPlay();
	initPlayerstart();
	SetupWorldSpawner();
	
}

void AArkdeCMGameMode::initPlayerstart()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* CurrentPlayerStart = *It;
		if (IsValid(CurrentPlayerStart))
		{
			WorldPlayerStart.Add(CurrentPlayerStart);
		}
	}


}



AActor* AArkdeCMGameMode::ChoosePlayerStart_Implementation(AController* player)
{
	APlayerStart* bestStart = nullptr;
	int32 RandomIndex = FMath::RandHelper(WorldPlayerStart.Num() - 1);

	if (RandomIndex >= 0 && WorldPlayerStart.Num() > 0)
	{
		bestStart = WorldPlayerStart[RandomIndex];
		WorldPlayerStart.RemoveAt(RandomIndex);
	}

	if (IsValid(bestStart))
	{
		return bestStart;
	}
	else
	{
		return Super::ChoosePlayerStart_Implementation(player);
	}
}

void AArkdeCMGameMode::SetupWorldSpawner()
{
	int CollectableIndex = 0;
	for (TActorIterator<AACM_Spawner> It(GetWorld()); It; ++It)
	{
		AACM_Spawner* CurrentSpawner = *It;
		CurrentSpawner->SetCollectableSubClass(SpawnableCollectables[CollectableIndex % SpawnableCollectables.Num()]);
		CurrentSpawner->RespawnCollectable();
		CollectableIndex++;

	}
}



