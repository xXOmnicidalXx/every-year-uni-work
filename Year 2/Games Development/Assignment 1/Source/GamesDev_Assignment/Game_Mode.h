// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Game_Mode.generated.h"

/**
 * 
 */
UCLASS()
class GAMESDEV_ASSIGNMENT_API AGame_Mode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGame_Mode();

	void GameOver();
	void Rounds();

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Game Settings")
		int RoundNumber;

protected:

private:
	void BeginPlay() override;

	APlayerController* Controller;

	int32 CurrentLevelIndex;
};
