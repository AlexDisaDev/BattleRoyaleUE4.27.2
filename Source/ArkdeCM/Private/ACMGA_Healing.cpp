// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMGA_Healing.h"
#include "ArkdeCM/ArkdeCMCharacter.h"
#include "ArkdeCM/Public/ACMT_PlayMontageAndWaitForEvent.h"
#include "ArkdeCM/Public/ACM_ActorAbilitySpawnOnBody.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

void UACMGA_Healing::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UACMT_playMontageAndWaitForEvent* montageTask = UACMT_playMontageAndWaitForEvent::playMontageAndWaitForEvent(
		this,
		NAME_None,
		CastMontage,
		FGameplayTagContainer(),
		1.f,
		NAME_None,
		false,
		1.f
	);

	montageTask->OnBlendOut.AddDynamic(this, &UACMGA_Healing::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACMGA_Healing::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACMGA_Healing::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACMGA_Healing::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACMGA_Healing::EventReceived);

	montageTask->ReadyForActivation();
}

void UACMGA_Healing::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

}

void UACMGA_Healing::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
	

}

void UACMGA_Healing::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FVector startLoc = character->GetMesh()->GetSocketLocation(AbilitySocketName);


		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(AbilitySocketName);

		FActorSpawnParameters spawnparams;
		spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AACM_ActorAbilitySpawnOnBody* powerPoint = GetWorld()->SpawnActorDeferred<AACM_ActorAbilitySpawnOnBody>(BodySpawnPower, spawnTransform, GetOwningActorFromActorInfo(), character,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		powerPoint->RangeH = ProjectileRange;

		powerPoint->FinishSpawning(spawnTransform);

		powerPoint->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AbilitySocketName);

		

	}

}
