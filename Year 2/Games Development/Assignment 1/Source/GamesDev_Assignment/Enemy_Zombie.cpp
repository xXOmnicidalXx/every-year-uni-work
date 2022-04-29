// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy_Zombie.h"
#include "DrawDebugHelpers.h"

// Sets default values
AEnemy_Zombie::AEnemy_Zombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = DeafualtHealth;

	DamageType = UDamageType::StaticClass();
	Damage = 50.0f;

	/// creating scene for raycast
	RayFireZ = CreateDefaultSubobject<USceneComponent>(TEXT("RayCast"));
	RayFireZ->SetupAttachment(RootComponent);
		   
	RayFireZ->SetRelativeLocation(FVector(0.0f, 25.0f, 18.0f));
	RayFireZ->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

}

// Called when the game starts or when spawned
void AEnemy_Zombie::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AEnemy_Zombie::HandleHit);
	
}

// Called every frame
void AEnemy_Zombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_Zombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy_Zombie::StartFireZ()
{
	FireZ();

	GetWorldTimerManager().SetTimer(TimerHandle_Handlefire, this, &AEnemy_Zombie::FireZ, 0.08f, true);
}

void AEnemy_Zombie::StopFireZ()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Handlefire);
}

void AEnemy_Zombie::FireZ()
{
	FHitResult ZHit;

	FVector Start = RayFireZ->GetComponentLocation();
	FVector ForwardVector = RayFireZ->GetForwardVector();

	Start = Start + (ForwardVector * 75.0f);
	FVector End = Start + (ForwardVector * 5000.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(ZHit, Start, End, ECC_Visibility, CollisionParams);

	if (IsHit)
	{
		AActor* HitActor = ZHit.GetActor();

		UGameplayStatics::ApplyPointDamage(HitActor, Damage, Start, ZHit, DamageInstigator, this, DamageType);

		DrawDebugBox(GetWorld(), ZHit.ImpactPoint, FVector(10, 10, 10), FColor::Yellow, false, 2.0f);
	}
}

void AEnemy_Zombie::HandleHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit by: "), *OtherActor->GetName());
}

float AEnemy_Zombie::TakeDamage(float DamageAmount, FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Health: "), Health);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
	}
	return DamageAmount;
}