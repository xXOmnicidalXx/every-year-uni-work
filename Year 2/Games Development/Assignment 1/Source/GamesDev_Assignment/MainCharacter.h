// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"	
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Grenade.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.generated.h"

//class USceneCaptureComponent2D;

UCLASS()
class GAMESDEV_ASSIGNMENT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()



public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	// U-PROPERTIES
	UPROPERTY(EditAnywhere, Category = "View")
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category = "View")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "View")
		float TurnSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Grenade")
		class USceneComponent* GrenadeSpawnPoint;
	UPROPERTY(EditAnywhere, CateGory = "Grenade")

		class TSubclassOf<AGrenade> Grenade;
	UPROPERTY(EditAnywhere, CateGory = "Damage")
		float GrenadeDamage;

	UPROPERTY()
		class AController* DamageInstigator;
	UPROPERTY(EditAnywhere, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, Category = "RayCast")
		class USceneComponent* RayFire;

	UPROPERTY(EditAnywhere, Category = "Map")
		class USpringArmComponent* MapArm;
	UPROPERTY(EditAnywhere, Category = "Map")
		class USceneCaptureComponent2D* MapCamera;

	// U-FUNCTIONS


	// FUNCTIONS
	void Forward(float Value);
	void Strafe(float Value);

	void StartCrouch();
	void StopCrouch();

	void Sprint();
	void StopSprint();

	void Yaw(float Value);
	void Pitch(float Value);

	void StartFire();
	void StopFire();
	void Fire();
	FTimerHandle TimerHandle_Handlefire;

	void StartThrow();
	void EndThrow();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	float Health = 0.0f;
	float DeafualtHealth = 100.0f;
};