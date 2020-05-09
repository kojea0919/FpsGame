// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfoInstance.h"
#include "Game/UI/MainMenuWidgetBase.h"
#include "Game/UI/CreateSessionWidgetBase.h"
#include "Game/UI/MatchingWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "fps.h"


UGameInfoInstance::UGameInfoInstance()
{
	//생성 Bind
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this,
		&UGameInfoInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this,
		&UGameInfoInstance::OnStartOnlineGameComplete);

	//검색 Bind
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this,
		&UGameInfoInstance::OnFindSessionsComplete);

	//참가 Bind
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this,
		&UGameInfoInstance::OnJoinSessionComplete);

	//파괴 Bind
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,
		&UGameInfoInstance::OnDestroySessionComplete);
}
void UGameInfoInstance::ShowMainMenu()
{
	if (nullptr == MainMenu)
	{
		MainMenu = CreateWidget<UMainMenuWidgetBase>(this, MainMenu_Template);
	}
	MainMenu->AddToViewport();
	
	APlayerController * pPc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == pPc)
	{
		UE_LOG(LogClass, Warning, TEXT("GameInfoInstance::ShowMainMenu / pPc nullptr"));
		return;
	}
	pPc->bShowMouseCursor = true;
}

void UGameInfoInstance::ShowMatchingMenu()
{
	if (nullptr == MatchingMenu)
	{
		MatchingMenu = CreateWidget<UMatchingWidgetBase>(this, MatchingMenu_Template);
	}
	MatchingMenu->AddToViewport();
}

void UGameInfoInstance::ShowCreateMenu()
{
	if (nullptr == CreateMenu)
	{
		CreateMenu = CreateWidget<UCreateSessionWidgetBase>(this, CreateMenu_Template);
	}
	CreateMenu->AddToViewport();
}


bool UGameInfoInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN , int32 MaxNumPlayers)
{
	IOnlineSubsystem * pOnlineSub = IOnlineSubsystem::Get();

	if (pOnlineSub)
	{
		IOnlineSessionPtr Sessions = pOnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//생성할 Session Setting
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;

			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	return false;
}

void UGameInfoInstance::StartOnlineGame(FText PlayerID)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FUniqueNetIdWrapper uniqueNetIdWrapper = FUniqueNetIdWrapper(Player->GetPreferredUniqueNetId());

	//UI에서 입력받은 ID로 PlayerID설정
	LocalPlayerID = PlayerID;

	HostSession(uniqueNetIdWrapper.GetUniqueNetId(), GameSessionName, true, 6);
}

void UGameInfoInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			//Session을 찾은 경우 JoinSession
			if (SessionSearch->SearchResults.Num() > 0)
			{
				ULocalPlayer* const Player = GetFirstGamePlayer();

				FUniqueNetIdWrapper uniqueNetIdWrapper = FUniqueNetIdWrapper(Player->GetPreferredUniqueNetId());
				JoinToSession(uniqueNetIdWrapper.GetUniqueNetId(), GameSessionName, SessionSearch->SearchResults[0]);
				
			}
		}
	}
}

void UGameInfoInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		OnFindSessionsComplete(false);
	}
}

void UGameInfoInstance::FindOnlineGames(bool bIsLAN, bool bIsPresence , FText PlayerID)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FUniqueNetIdWrapper uniqueNetIdWrapper = FUniqueNetIdWrapper(Player->GetPreferredUniqueNetId());

	LocalPlayerID = PlayerID;

	FindSessions(uniqueNetIdWrapper.GetUniqueNetId(), GameSessionName, bIsLAN, bIsPresence);
}

void UGameInfoInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
			APlayerController * const PlayerController = GetFirstLocalPlayerController();
		
			FString TravelURL;
			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

bool UGameInfoInstance::JoinToSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult & SearchResult)
{
	bool bSuccessful = false;
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}

void UGameInfoInstance::DestroySession()
{
	IOnlineSubsystem * OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UGameInfoInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "MainMenu",true);
			}
		}
	}
}

void UGameInfoInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UGameInfoInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Arena01", true, "listen");
	}
}
