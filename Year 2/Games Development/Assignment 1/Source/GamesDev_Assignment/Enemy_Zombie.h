// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_Zombie.generated.h"

UCLASS()
class GAMESDEV_ASSIGNMENT_API AEnemy_Zombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Zombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//UPROPERTY
	UPROPERTY(EditAnywhere, Category = "RayCast")
		class USceneComponent* RayFireZ;

	UPROPERTY()
		class AController* DamageInstigator;
	UPROPERTY(EditAnywhere, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category = "Damage")
		float Damage;

	//UFUNCTION
	UFUNCTION()
		void HandleHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void StartFireZ();
	void StopFireZ();
	void FireZ();
	FTimerHandle TimerHandle_Handlefire;


	virtual float TakeDamage(float DamageAmount, FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	float Health = 0.0f;
	float DeafualtHealth = 500.0f;
};