// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"

//class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GAMES_ASSIGNMENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//UPROPERTY(VisibleAnywhere)
		//USkeletalMeshComponent* CharacterMesh;
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;

	void SetMoveAmount(float Value);
	void SetStrafeAmount(float Value);
	void SetYawAmount(float Value);
	void SetPitchAmount(float Value);

	FVector DeltaLocation = FVector(0.0f, 0.0f, 0.0f);
	FQuat DeltaRotation = FQuat(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
		float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere)
		float RotationSpeed = 200.0f;

};
