// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TeaBag.generated.h"

class UProjectileMovementComponent;

UCLASS()
class TEAPOTLABS_API ATeaBag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeaBag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TeaBagMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 600.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
