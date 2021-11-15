// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"	
#include "GameFramework/SpringArmComponent.h"
#include "TeaBag.h"
#include "TeaPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;

UCLASS()
class TEAPOTLABS_API ATeaPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATeaPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PawnMesh;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ProjectileSpawnPoint;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ATeaBag> TeaBagClass;


	void SetMoveAmount(float Value);
	void SetStrafeAmount(float value);
	void SetRotateAmount(float Value);
	void OnBeginFire();
	void OnEndFire();

	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	FQuat DeltaRotation = FQuat(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
		float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere)
		float RotationSpeed = 200.0f;
};