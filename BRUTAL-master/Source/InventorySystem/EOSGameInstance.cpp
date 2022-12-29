// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "OnlineSessionSettings.h"

const FName TestSessionName = FName("Test Session");

UEOSGameInstance::UEOSGameInstance()
{
	bIsLoggedIn = false;
}

void UEOSGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	Login();
}

void UEOSGameInstance::Login()
{
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			FOnlineAccountCredentials Credentials;
			Credentials.Id = FString();
			Credentials.Token = FString();
			Credentials.Type = FString("accountportal");
			Identity->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::OnLoginComplete);
			Identity->Login(0, Credentials);
		}
	}
}

void UEOSGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("Call from logincomplete:: LoggedIn: %d Completed Successful Error String = %s"), bWasSuccessful,*ErrorString);
	bIsLoggedIn = bWasSuccessful;
	if (OnlineSubsystem)
	{
		if (IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface())
		{
			Identity->ClearOnLoginCompleteDelegates(0,this);
		}
	}
}

void UEOSGameInstance::StartSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				UE_LOG(LogTemp, Warning, TEXT("HHHHHHHHHvoid UEOSGameInstance::StartSession()HHHHHHHHHHHHHHHHH"));
				//SessionPtr->OnStartSessionCompleteDelegates
				SessionPtr->GetSessionState(TestSessionName);
				SessionPtr->StartSession(TestSessionName);
				FString mytempinfo = SessionPtr->GetNamedSession(TestSessionName)->SessionInfo.Get()->ToDebugString();
				SessionPtr->OnStartSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::StartSessionCompleted);
				SessionPtr->DumpSessionState();
				UE_LOG(LogTemp, Warning, TEXT("Session Session Info: %s"), *mytempinfo);
			}
		}
	}
}

void UEOSGameInstance::StartSessionCompleted(FName n, bool b)
{
	UE_LOG(LogTemp, Warning, TEXT("StartSessionCompleted Success?: %d And name %s "), b,*n.ToString());

}

void UEOSGameInstance::CreateSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;
				
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings.bAllowInvites = true;
				SessionSettings.BuildUniqueId = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;
				
				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSessionComplete);
				SessionPtr->CreateSession(0, TestSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT LOGGED IN CREATE SESSION FAILED"));
	}
	
}

void UEOSGameInstance::CreateSeemlessSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;

				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings.bAllowInvites = true;
				SessionSettings.BuildUniqueId = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateSeemlessSessionComplete);
				SessionPtr->CreateSession(0, TestSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT LOGGED IN CREATE SESSION FAILED"));
	}

}

void UEOSGameInstance::OnCreateSeemlessSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d Session Created succesfully"), bWasSuccessful);
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
			//SessionPtr->info
			GetWorld()->ServerTravel(FString("ALScontent/ALS_DemoLevelModded?listen"), true);
			//SessionPtr->info

			UE_LOG(LogTemp, Warning, TEXT("Success: %d Session Created succesfully"), bWasSuccessful);
		}
	}
}

void UEOSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d Session Created succesfully"), bWasSuccessful);
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
			//SessionPtr->info
			GetWorld()->ServerTravel(FString("ALScontent/ALS_DemoLevelModded?listen"), false);
			//SessionPtr->info
			//UGameplayStatics::OpenLevel()
			UE_LOG(LogTemp, Warning, TEXT("Success: %d Session Created succesfully"), bWasSuccessful);
		}
	}
}

void UEOSGameInstance::OpenLevelSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;

				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings.bAllowInvites = true;
				SessionSettings.BuildUniqueId = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnOpenLevelSessionComplete);
				SessionPtr->CreateSession(0, TestSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT LOGGED IN CREATE SESSION FAILED"));
	}
}

void UEOSGameInstance::OnOpenLevelSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d Session Created succesfully"), bWasSuccessful);
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
			//SessionPtr->info
			//GetWorld()->ServerTravel(FString("ALScontent/ALS_DemoLevelModded?listen"), false);
			//SessionPtr->info
			//UGameplayStatics::OpenLevel(this, *FString("ALScontent/ALS_DemoLevelModded?listen"));
			UGameplayStatics::OpenLevel(GetWorld(), *FString("test"),true,"listen");
			UE_LOG(LogTemp, Warning, TEXT("Success: %d Session OpenLevel succesfully"), bWasSuccessful);
		}
	}
}

void UEOSGameInstance::JoinMAXSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);


				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnMAXJoinSessionComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());
			}
		}
	}
}

void UEOSGameInstance::OnMAXJoinSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Seemless: Success: %d"), bWasSuccessful);
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Seemless: Found %d Lobbies"), SearchSettings->SearchResults.Num());
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				if (SearchSettings->SearchResults.Num())
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnMAXClientTravelJoinSessionComplete);
					SessionPtr->JoinSession(0, TestSessionName, SearchSettings->SearchResults[0]);
				}
			}
		}
	}
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::OnMAXClientTravelJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName, ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					
					UE_LOG(LogTemp, Warning, TEXT(" UEOSGameInstance::OnMAXClientTravelJoinSessionComplete ::: %s"),*ConnectionInfo);
					PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_MAX);
					//SessionPtr->R
				}
			}
		}
	}
}

void UEOSGameInstance::DestroySession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SessionPtr->OnDestroySessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnDestroySessionComplete);
				SessionPtr->DestroySession(TestSessionName);
			}
		}
	}

}

void UEOSGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnDestroySessionComplete"));
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnDestroySessionCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::FindSessions()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				SearchSettings = MakeShareable(new FOnlineSessionSearch());
				SearchSettings->MaxSearchResults = 5000;
				SearchSettings->QuerySettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineComparisonOp::Equals);
				SearchSettings->QuerySettings.Set(SEARCH_LOBBIES,true, EOnlineComparisonOp::Equals);


				SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnFindSessionsComplete);
				SessionPtr->FindSessions(0, SearchSettings.ToSharedRef());
			}
		}
	}
}

void UEOSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d"), bWasSuccessful);
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d Lobbies"), SearchSettings->SearchResults.Num());
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				if (SearchSettings->SearchResults.Num())
				{
					SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnJoinSessionComplete);
					SessionPtr->JoinSession(0, TestSessionName, SearchSettings->SearchResults[0]);
				}
			}
		}
	}
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnFindSessionsCompleteDelegates(this);
		}
	}
}

void UEOSGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			FString ConnectionInfo = FString();
			SessionPtr->GetResolvedConnectString(SessionName, ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					PC->ClientTravel(ConnectionInfo, ETravelType::TRAVEL_Absolute);
					UE_LOG(LogTemp, Warning, TEXT("UEOSGameInstance::OnJoinSessionComplete:: %s"), *ConnectionInfo);
					//SessionPtr->R
				}
			}
		}
	}
}

void UEOSGameInstance::GetAllFriends()
{
	UE_LOG(LogTemp, Warning, TEXT("Get All Friends Pressed"));
	if (bIsLoggedIn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get All Friends Pressed bIsLoggedIn"));
		if (OnlineSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Get All Friends Pressed Onlinesubsystems"));
			if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				UE_LOG(LogTemp, Warning, TEXT("Get All Friends Pressed Onlinesubsystems FriendsPtr"));

				FriendsPtr->ReadFriendsList(0, FString(""), FOnReadFriendsListComplete::CreateUObject(this, &UEOSGameInstance::OnGetAllFriendsComplete));
			}
		}
	}
}

void UEOSGameInstance::OnGetAllFriendsComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UE_LOG(LogTemp, Warning, TEXT("Value of bWasSuccessful = %d in UEOSGameInstance::OnGetAllFriendsComplete ErrorStr = %s"), bWasSuccessful, *ErrorStr);
	if (bWasSuccessful)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineFriendsPtr FriendsPtr = OnlineSubsystem->GetFriendsInterface())
			{
				TArray<TSharedRef<FOnlineFriend>> FriendsList;
				if (FriendsPtr->GetFriendsList(0, ListName, FriendsList))
				{
					for (TSharedRef<FOnlineFriend> Friend : FriendsList)
					{
						FString FriendName = Friend.Get().GetRealName();
						UE_LOG(LogTemp, Warning, TEXT("Friend is = %s"), *FriendName);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed GetFriendsList"));
				}				
			}
		}
	}
}

void UEOSGameInstance::ShowFriendsUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowFriendsUI(0);
			}
		}
	}
}

void UEOSGameInstance::ShowInviteUI()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineExternalUIPtr UIPtr = OnlineSubsystem->GetExternalUIInterface())
			{
				UIPtr->ShowInviteUI(0,TestSessionName);
			}
		}
	}
}

void UEOSGameInstance::Registerplayer()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				//First get a reference to whichever game player you need, in this case the local one
				const TSharedPtr<const FUniqueNetId> netID = UGameplayStatics::GetGameInstance(GetWorld())->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
				//GetFirstLocalPlayerController(GetWorld())->NetID;
				//SessionPtr->OnRegisterPlayersCompleteDelegates
				SessionPtr->RegisterPlayer(TestSessionName,*netID, 1);
				//SessionPtr->OnR
			}
		}
	}
}

void UEOSGameInstance::OnRegisteredPlayerComplete()
{

}

void UEOSGameInstance::CreateDedicatedSession()
{
	if (bIsLoggedIn)
	{
		if (OnlineSubsystem)
		{
			if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsDedicated = false;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bIsLANMatch = false;
				SessionSettings.NumPublicConnections = 5;

				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.Set(SEARCH_KEYWORDS, FString("YTTutorialLobby"), EOnlineDataAdvertisementType::ViaOnlineService);
				SessionSettings.bAllowInvites = true;
				SessionSettings.BuildUniqueId = true;
				SessionSettings.bUseLobbiesVoiceChatIfAvailable = true;

				SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::OnCreateDedicatedSessionComplete);
				SessionPtr->CreateSession(0, TestSessionName, SessionSettings);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NOT LOGGED IN CREATE SESSION FAILED"));
	}
}

void UEOSGameInstance::OnCreateDedicatedSessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Success: %d Dedicated Session Created succesfully"), bWasSuccessful);
	if (OnlineSubsystem)
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
			//SessionPtr->info
			GetWorld()->ServerTravel(FString("ALScontent/ALS_DemoLevelModded?listen"), false);
			//SessionPtr->info

			UE_LOG(LogTemp, Warning, TEXT("Success: %d Dedicated Session Created succesfully"), bWasSuccessful);
		}
	}
}
