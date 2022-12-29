// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "SpectatorPawnWarGame.generated.h"
class UItemNameDisplayer;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class INVENTORYSYSTEM_API ASpectatorPawnWarGame : public ADefaultPawn
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UItemNameDisplayer* ItemName = nullptr;

	FHitResult Hit;
	virtual void Tick(float DeltaTime) override;
	void LineTrace();

	UPROPERTY(EditAnywhere, Category = "UII")
	TSubclassOf<UItemNameDisplayer> ItemNameWidgetClass = nullptr;
	void PossessPawn();
	ASpectatorPawnWarGame();
	/** PostLoad override. */


};
