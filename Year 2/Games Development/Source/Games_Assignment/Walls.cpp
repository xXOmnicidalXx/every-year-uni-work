// Fill out your copyright notice in the Description page of Project Settings.


#include "Walls.h"

// Sets default values
AWalls::AWalls()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Mesh"));
	WallMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWalls::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

