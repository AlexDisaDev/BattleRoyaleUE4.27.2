// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArkdeCMGameMode.generated.h"

class APlayerStart;
class AACM_Collectable;

UCLASS(minimalapi)
class AArkdeCMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AArkdeCMGameMode();

	void playerKilled(AController* VictimController);

	virtual void BeginPlay() override;

	TArray<APlayerStart*> WorldPlayerStart;

	void initPlayerstart();

	virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<AACM_Collectable>> SpawnableCollectables;

	void SetupWorldSpawner();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_DestroyStorm();

	UPROPERTY(BlueprintReadWrite)
	int playerAlives;

};