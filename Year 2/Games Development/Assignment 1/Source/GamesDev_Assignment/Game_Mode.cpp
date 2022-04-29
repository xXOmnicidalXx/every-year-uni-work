// Fill out your copyright notice in the Description page of Project Settings.


#include "Game_Mode.h"

AGame_Mode::AGame_Mode()
{
	RoundNumber = 100;
	CurrentLevelIndex = 0;
}

void AGame_Mode::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputMode;
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = false;
}

void AGame_Mode::GameOver()
{
	// if main character health is 0
}

void AGame_Mode::Rounds()
{
	// if all enemies are dead, start next round
	
	//multiple rounds (countless)

	// difficulty increasing as rounds increase

	// enemy stats base
	// spawn 8 (+8 each round)
	// health 5 (+ 55 each round)
	// dmg 9 (+9 each round)
	// speed 300 (+77.7 each round)

	// max stats
	// spawn 200 (25 rounds)
	// health 500 (10 rounds)
	// dmg 90 (10 rounds)
	// speed 1000 (10 rounds)

	// stats
	// 10 points per hit
	// 100 points perk kill
}


