// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameController.generated.h"

class UUserWidget;

UCLASS()
class GAMESDEV_ASSIGNMENT_API AGameController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MiniMapCLass;
		
	UUserWidget* MiniMap;

public:

	
};
