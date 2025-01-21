// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_PlayerController.h"
#include "Kismet/GameplayStatics.h"


AACM_PlayerController::AACM_PlayerController()
{
	bGameConclussion = false;
}

void AACM_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	SetViewTarget(aPawn);
}

void AACM_PlayerController::GameConclussion(bool WasSuccesful)
{
    
		BP_GameConclussion(WasSuccesful);

		TArray<AActor*> ViewPointActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ViewActorClass, ViewPointActors);

		SetViewTargetWithBlend(ViewPointActors[0], 0.5f, EViewTargetBlendFunction::VTBlend_Linear);
		
		

}
