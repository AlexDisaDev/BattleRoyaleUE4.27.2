// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_GameInstance.h"
#include "ACM_PlayerController.h"
#include "ACM_PlayerState.h"
#include "ACM_LobbyGameMode.h"

void UACM_GameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	

	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::init Subsytem Found %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();

		if (SessionInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::init session interface found"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionCreated);
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionDestroyed);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionFound);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionJoined);
		}
	}
}

void UACM_GameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Host session Start"));
	auto CreatedSession = SessionInterface->GetNamedSession(SessionDefaultName);
	if (CreatedSession)
	{
		SessionInterface->DestroySession(SessionDefaultName);

	}
	else
	{
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
		{
			SessionSettings.bIsLANMatch = true; //booleano para activar el match en lan
		}
		else
		{
			SessionSettings.bIsLANMatch = false; //booleano para activar el match en lan
		}
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bShouldAdvertise = true; // hacer que la sala no sea privada , false para que la sala sea privada
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.NumPublicConnections = 100; //activar el numero de busquedas publicas
		SessionSettings.Set(TEXT("ServerName"), FString(TEXT("ArkdeBR")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
	}

}

void UACM_GameInstance::Join()
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Join find session"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
	{
		SessionSearch->bIsLanQuery = true;
	}
	else
	{
		SessionSearch->bIsLanQuery = false;
	}


	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::NotEquals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UACM_GameInstance::OnSessionCreated(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Host session ends"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionCreated sesion fail"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionCreated session Success"));
	UWorld* GameWorld = GetWorld();
	GameWorld->ServerTravel("/Game/BP/Maps/PreBR?listen");
}

void UACM_GameInstance::OnSessionDestroyed(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed session ends"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed session failed"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed session destroy success"));
	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
	{
		SessionSettings.bIsLANMatch = true; //booleano para activar el match en lan
	}
	else
	{
		SessionSettings.bIsLANMatch = false; //booleano para activar el match en lan
	}

	SessionSettings.bShouldAdvertise = true; // hacer que la sala no sea privada , false para que la sala sea privada
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.NumPublicConnections = 100; // activar el numero de busquedas publicas
	SessionSettings.Set(TEXT("ServerName"), FString(TEXT("ArkdeBR")), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
}

void UACM_GameInstance::OnSessionFound(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound find session end"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound session Failed"));
		return;
	}
	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (FOnlineSessionSearchResult CurrentResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound session found %s"), *CurrentResult.GetSessionIdStr());
		}
		SessionInterface->JoinSession(0, SessionDefaultName, SessionSearch->SearchResults[0]);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound no session found"));
	}
}

void UACM_GameInstance::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined join session end"));

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined join session fail"));
		return;
	}
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined no player controller found"));
		return;
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}



