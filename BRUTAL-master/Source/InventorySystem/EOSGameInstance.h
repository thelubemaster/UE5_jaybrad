// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEOSGameInstance();

	virtual void Init() override;

	void Login();
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString);

	class IOnlineSubsystem* OnlineSubsystem;

	UFUNCTION(BlueprintCallable)
	void StartSession();
	void StartSessionCompleted(FName n, bool b);

	UFUNCTION(BlueprintCallable)
	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void OpenLevelSession();
	void OnOpenLevelSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void CreateSeemlessSession();
	void OnCreateSeemlessSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void JoinMAXSession();
	void OnMAXJoinSessionComplete(bool bWasSuccessful);
	void OnMAXClientTravelJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void FindSessions();
	void OnFindSessionsComplete(bool bWasSuccessful);
	TSharedPtr<FOnlineSessionSearch> SearchSettings;
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UFUNCTION(BlueprintCallable)
	void GetAllFriends();
	void OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	UFUNCTION(BlueprintCallable)
	void ShowFriendsUI();
	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();

	UFUNCTION(BlueprintCallable)
	void Registerplayer();
	void OnRegisteredPlayerComplete();


	UFUNCTION(BlueprintCallable)
	void CreateDedicatedSession();
	void OnCreateDedicatedSessionComplete(FName SessionName, bool bWasSuccessful);

	bool bIsLoggedIn;
};
