// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMGA_Firebolt.h"
#include "ArkdeCM/ArkdeCMCharacter.h"
#include "ArkdeCM/Public/ACMT_PlayMontageAndWaitForEvent.h"
#include "ArkdeCM/Public/ACM_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"

void UACMGA_Firebolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
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

	montageTask->OnBlendOut.AddDynamic(this, &UACMGA_Firebolt::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACMGA_Firebolt::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACMGA_Firebolt::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACMGA_Firebolt::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACMGA_Firebolt::EventReceived);

	montageTask->ReadyForActivation();
}

//=====================================================================================================
void UACMGA_Firebolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
//=====================================================================================================
void UACMGA_Firebolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
//=====================================================================================================
void UACMGA_Firebolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FVector startLoc = character->GetMesh()->GetSocketLocation(AbilitySocketName);
		FVector endLoc = character->GetCameraBoom()->GetComponentLocation() + (character->GetFollowCamera()->GetForwardVector() * ProjectileRange);
		FRotator projectileRotation = UKismetMathLibrary::FindLookAtRotation(startLoc, endLoc);

		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(AbilitySocketName);
		spawnTransform.SetRotation(projectileRotation.Quaternion());

		FActorSpawnParameters spawnparams;
		spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AACM_Projectile* firebolt = GetWorld()->SpawnActorDeferred<AACM_Projectile>(ProjectileClass, spawnTransform, GetOwningActorFromActorInfo(), character,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if(IsValid(firebolt))
		firebolt->Multicast_IgnoreActor(character);
		firebolt->Range = ProjectileRange;

		if (IsValid(firebolt->SphereComponent))
		{
			firebolt->SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		firebolt->FinishSpawning(spawnTransform);

	}


}
//=====================================================================================================
