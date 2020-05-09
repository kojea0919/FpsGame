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

	
	//Session����
	//----------------------
	//Session������ ȣ��� Delegate
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//Session���۽� ȣ��� Delegate
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	//������ Session�� Setting
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	//Create�� �� ������ �Լ�
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN , int32 MaxNumPlayers);

	UFUNCTION(BlueprintCallable)
	void StartOnlineGame(FText PlayerID);
	//----------------------
	//Session����



	//Session�˻�
	//----------------------

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	void OnFindSessionsComplete(bool bWasSuccessful);

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


	//Find�� �� ������ �Լ�
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence);

	UFUNCTION(BlueprintCallable)
	void FindOnlineGames(bool bIsLAN, bool bIsPresence, FText PlayerID);

	//----------------------
	//Session�˻�



	//Session����
	//----------------------
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	//Join�� �� ������ �Լ�
	bool JoinToSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//----------------------
	//Session����
	
	
	//Session�ı�
	//----------------------
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//----------------------
	//Session�ı�

	FText LocalPlayerID;

};