// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Main_AIController.generated.h"

/**
 * 
 */
UCLASS()
class GAMESDEV_ASSIGNMENT_API AMain_AIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:


};
