// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkdeCMCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "ACM_AttributeSet.h"
#include "ACM_GameplayAbility.h"
#include "ArkdeCM/ArkdeCM.h"
#include "ArkdeCM/Public/ACM_PlayerState.h"
#include "ArkdeCM/ArkdeCMGameMode.h"


//////////////////////////////////////////////////////////////////////////
// AArkdeCMCharacter

AArkdeCMCharacter::AArkdeCMCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	IsInputbound = false;
	AbilitiesGiven = false;
	EffectsGiven = false;
	IsDying = false;
}

//=====================================================================================================

void AArkdeCMCharacter::BeginPlay()
{
	Super::BeginPlay();
}
//=====================================================================================================

void AArkdeCMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AACM_PlayerState* playerState = GetPlayerState<AACM_PlayerState>();

	if (IsValid(playerState))
	{
		AbilitySystemComponenet = playerState->GetAbilitySystemComponent();
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
		AttributeSet = playerState->GetAttributeSet();
		AttributeSet->OnhealthChangedelegate.AddDynamic(this, &AArkdeCMCharacter::OnCharacterhealthChanged);
		AttributeSet->OnMAnaChangeDelegate.AddDynamic(this, &AArkdeCMCharacter::OnCharacterManaChanged);

		SetupAbilities();
		SetupEffects();
	}
}

//////////////////////////////////////////////////////////////////////////
//  Input

void AArkdeCMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArkdeCMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArkdeCMCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArkdeCMCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArkdeCMCharacter::LookUpAtRate);

	//Setup ASC input bindings
	SetupGasinputs();
}
UAbilitySystemComponent* AArkdeCMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponenet;
}
//=====================================================================================================

void AArkdeCMCharacter::SetupGasinputs()
{
	if (!IsInputbound && IsValid(AbilitySystemComponenet) && IsValid(InputComponent))
	{
		AbilitySystemComponenet->BindAbilityActivationToInputComponent(
			InputComponent,
			FGameplayAbilityInputBinds(
				"Confirm",
				"Cancel",
				"EACM_AbilityInputID",
				static_cast<int32>(EACM_AbilityInputID::Confirm),
				static_cast<int32>(EACM_AbilityInputID::Cancel)
			)
		);
		IsInputbound = true;
	}
}

//=====================================================================================================

void AArkdeCMCharacter::SetupAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponenet) || AbilitiesGiven)
	{
		return;
	}
	for (TSubclassOf<UACM_GameplayAbility>& currentAbility : StartingAbilities)
	{
		if (IsValid(currentAbility))
		{
			UACM_GameplayAbility* defaultObj = currentAbility->GetDefaultObject<UACM_GameplayAbility>();
			FGameplayAbilitySpec abilitySpec(defaultObj, 1, static_cast<int32>(defaultObj->AbilityInputID), this);
			AbilitySystemComponenet->GiveAbility(abilitySpec);
		}
	}

	AbilitiesGiven = true;
}
//=====================================================================================================

void AArkdeCMCharacter::SetupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponenet) || EffectsGiven)
	{
		return;
	}

	FGameplayEffectContextHandle effectContext = AbilitySystemComponenet->MakeEffectContext();
	effectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& currentEffect : StartingEffects)
	{
		if (IsValid(currentEffect))
		{
			FGameplayEffectSpecHandle newHandle = AbilitySystemComponenet->MakeOutgoingSpec(currentEffect, 1.f, effectContext);
			if (newHandle.IsValid())
			{
				FActiveGameplayEffectHandle activeGEHandle = AbilitySystemComponenet->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), GetAbilitySystemComponent());
			}
		}
	}

	EffectsGiven = true;

}
//=====================================================================================================

void AArkdeCMCharacter::serverDie_Implementation(AArkdeCMCharacter* killer)
{

	if (IsDying)
	{
		return;
	}
	IsDying = true;

	if (IsValid(DeathEffectClass))
	{
		FGameplayEffectContextHandle EffectContext;
		AbilitySystemComponenet->ApplyGameplayEffectToSelf(DeathEffectClass->GetDefaultObject<UGameplayEffect>(), 1.f, EffectContext);
	}


	AACM_PlayerState* KillerPlayerstate = Cast<AACM_PlayerState>(killer->GetPlayerState());
	if (IsValid(KillerPlayerstate))
	{
		KillerPlayerstate->ScoreKill();
	}

	AArkdeCMGameMode* WorldGameMode = Cast<AArkdeCMGameMode>(GetWorld()->GetAuthGameMode());
	WorldGameMode->playerKilled(GetController());

	Multicast_OnDeath();
}
//=====================================================================================================

void AArkdeCMCharacter::Multicast_OnDeath_Implementation()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
		
	}
	else
	{
		Destroy();
	}
}

//=====================================================================================================

void AArkdeCMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AACM_PlayerState* playerState = GetPlayerState<AACM_PlayerState>();
	if (IsValid(playerState))
	{
		AbilitySystemComponenet = playerState->GetAbilitySystemComponent();
		playerState->GetAbilitySystemComponent()->InitAbilityActorInfo(playerState, this);
		AttributeSet = playerState->GetAttributeSet();
		AttributeSet->OnhealthChangedelegate.AddDynamic(this, &AArkdeCMCharacter::OnCharacterhealthChanged);
		AttributeSet->OnMAnaChangeDelegate.AddDynamic(this, &AArkdeCMCharacter::OnCharacterManaChanged);
		SetupGasinputs();
	}

}


//=====================================================================================================

void AArkdeCMCharacter::OnCharacterhealthChanged(float health, float maxHealth)
{
	Client_healthChanged(health, maxHealth);
}

void AArkdeCMCharacter::Client_healthChanged_Implementation(float health, float maxhealth)
{
	OnCharacterhealthDelegate.Broadcast(health / maxhealth);
}

void AArkdeCMCharacter::OnCharacterManaChanged(float mana, float maxmana)
{
	Client_ManaChanged(mana, maxmana);
}

void AArkdeCMCharacter::Client_ManaChanged_Implementation(float mana, float maxmana)
{
	OnCharacterManaDelegate.Broadcast(mana / maxmana);
}

//=====================================================================================================

void AArkdeCMCharacter::OnResetVR()
{
	// If ArkdeCM is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in ArkdeCM.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AArkdeCMCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AArkdeCMCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AArkdeCMCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AArkdeCMCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AArkdeCMCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

}

void AArkdeCMCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
