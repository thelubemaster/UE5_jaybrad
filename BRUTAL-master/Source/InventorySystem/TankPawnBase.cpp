// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawnBase.h"

// Sets default values
ATankPawnBase::ATankPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankPawnBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATankPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

