// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Blueprint/UserWidget.h"

void AGameController::BeginPlay()
{
	Super::BeginPlay();

	MiniMap = CreateWidget(this, MiniMapCLass);

	if (MiniMap != nullptr)
	{
		MiniMap->AddToViewport();
	}
}