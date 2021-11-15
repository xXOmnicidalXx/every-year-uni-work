// Fill out your copyright notice in the Description page of Project Settings.


#include "TeaPot.h"

// Sets default values
ATeaPot::ATeaPot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Actor Mesh"));
	ActorMesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void ATeaPot::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));

	FVector Location = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Actor Location: X: %f Y: %f Z: %f "), Location.X,
		Location.Y, Location.Z);

	
}

// Called every frame
void ATeaPot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Hello World!"));

	FRotator Rotation = FRotator(0, RotationSpeed*DeltaTime, 0);
	AddActorLocalRotation(Rotation, true);

	FVector Movement = FVector(0, MovementSpeed * DeltaTime, 0);
	AddActorLocalOffset(Movement, true);
}

