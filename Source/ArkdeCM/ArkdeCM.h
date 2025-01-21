// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
UENUM(BlueprintType)
enum class EACM_AbilityInputID : uint8
{
	// 0 none
	None UMETA(DisplayName = "None"),
	// 1 confirm
	Confirm UMETA(DisplayName = "Confirm"),
	// 2 cancel
	Cancel UMETA(DisplayName = "Cancel"),
	// 3 Ability1 
	Ability1 UMETA(DisplayName = "Ability1"),
	// 4 Ability2
	Ability2 UMETA(DisplayName = "Ability2"),
	// 7 ability 3
	Ability3 UMETA(DisplayName = "Ability3"),
	// 6 Jump
	Jump UMETA(DisplayName = "Jump"),
};