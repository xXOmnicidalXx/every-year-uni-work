// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Grenade.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* GrenadeMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Projectile Speed")
		float StationaryMovementSpeed = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Speed")
		float StandardMovementSpeed = 1200.0f;
	UPROPERTY(EditAnywhere, Category = "Projectile Speed")
		float MaxMovementSpeed = 1800.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
