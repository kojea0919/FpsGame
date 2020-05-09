// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "GameInfoInstance.generated.h"

/**
 * 
 */

#define SETTING_SERVER_NAME FName(TEXT("SERVERNAMEKEY"))
#define SETTING_SERVER_IS_PROTECTED FName(TEXT("SERVERSERVERISPASSWORDPROTECTEDKEY"))
#define SETTING_SERVER_PROTECT_PASSWORD FName(TEXT("SERVERPROTECTPASSWORDKEY"))

UCLASS()
class FPS_API UGameInfoInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameInfoInstance();

	//UI
	//-------------------------------------------------
	
	//Main
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="UI")
	class UMainMenuWidgetBase * MainMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UMainMenuWidgetBase> MainMenu_Template;

	//Matching
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UMatchingWidgetBase * MatchingMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UMatchingWidgetBase> MatchingMenu_Template;

	//Create
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "UI")
	class UCreateSessionWidgetBase * CreateMenu;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	TSubclassOf<class UCreateSessionWidgetBase> CreateMenu_Template;
	//-------------------------------------------------



	//Show UI
	//-------------------------------------------------
	void ShowMainMenu();
	void ShowMatchingMenu();
	void ShowCreateMenu();
	//-------------------------------------------------

	
	//Session생성
	//----------------------
	//Session생성시 호출될 Delegate
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//Session시작시 호출될 Delegate
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	//생성할 Session을 Setting
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	//Create할 때 제공할 함수
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN , int32 MaxNumPlayers);

	UFUNCTION(BlueprintCallable)
	void StartOnlineGame(FText PlayerID);
	//----------------------
	//Session생성



	//Session검색
	//----------------------

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	void OnFindSessionsComplete(bool bWasSuccessful);

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


	//Find할 때 제공할 함수
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);

	UFUNCTION(BlueprintCallable)
	void FindOnlineGames(bool bIsLAN, bool bIsPresence, FText PlayerID);

	//----------------------
	//Session검색



	//Session참가
	//----------------------
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	//Join할 때 제공할 함수
	bool JoinToSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//----------------------
	//Session참가
	
	
	//Session파괴
	//----------------------
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//----------------------
	//Session파괴

	FText LocalPlayerID;

};