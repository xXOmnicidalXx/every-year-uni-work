// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeaPot.generated.h"

UCLASS()
class TEAPOTLABS_API ATeaPot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeaPot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ActorMesh;

	UPROPERTY(EditAnywhere)
		float RotationSpeed = 50.0f;

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 100.0f;
};
