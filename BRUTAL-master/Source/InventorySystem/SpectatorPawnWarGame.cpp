// Fill out your copyright notice in the Description page of Project Settings.

//#include "ModdedALSCharacter.h"
#include "SpectatorPawnWarGame.h"
#include "ModdedALSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/ItemNameDisplayer.h"

void ASpectatorPawnWarGame::Tick(float DeltaTime)
{
	LineTrace();
}

void ASpectatorPawnWarGame::LineTrace()
{
	if (IsLocallyControlled())
	{
		FVector Loc;
		FRotator Rot;

		GetController()->GetPlayerViewPoint(Loc, Rot);
		const FCollisionQueryParams TraceParams;
		const FVector Start = Loc;
		const FVector End = Start + (Rot.Vector() * 2000);

		const bool bHit2 = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_PhysicsBody, TraceParams);
		
		//ItemName->SetTextName(FText::FromString(*Hit.GetActor()->GetName()));
		if (bHit2 && Hit.GetActor())
		{
			//Hit.GetActor()->GetName()
			
				if (Cast<AModdedALSCharacter>(Hit.GetActor()))
				{
					
				}
		}
		//if(!Hit.GetActor()->GetName().IsEmpty())
		//ItemName->SetTextName(FText::FromString(*Hit.GetActor()->GetName()));
	}
}

void ASpectatorPawnWarGame::PossessPawn()
{

}

ASpectatorPawnWarGame::ASpectatorPawnWarGame()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (ItemNameWidgetClass)
	{
		ItemName = CreateWidget<UItemNameDisplayer>(PC, ItemNameWidgetClass);
		ItemName->AddToViewport();
	}

}


