// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Grenade.h"
#include "HealthComponent.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;
	GrenadeDamage = 20.0f;

	TurnSpeed = 100.0f;

	// spring arm & camera settings
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeLocation(FVector(-60.0f, 60.0f, 108.0f));
	SpringArm->SetRelativeRotation(FRotator(0.0f, -10.0f, 0.0f));
	SpringArm->TargetArmLength = 150.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	//render mini map
	MapArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MapArm"));
	MapArm->SetupAttachment(RootComponent);
	MapArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	MapArm->TargetArmLength = 300.0f;

	MapCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MapCamera"));
	MapCamera->SetupAttachment(MapArm);

	/// <summary>
	RayFire = CreateDefaultSubobject<USceneComponent>(TEXT("RayCast"));
	RayFire->SetupAttachment(RootComponent);
	
	RayFire->SetRelativeLocation(FVector(0.0f, 25.0f, 18.0f));
	RayFire->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	//
	GrenadeSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	GrenadeSpawnPoint->SetupAttachment(RootComponent);
	GrenadeSpawnPoint->SetRelativeLocation(FVector(10.0f, 10.0f, 45.0f));
	GrenadeSpawnPoint->SetRelativeRotation(FRotator(0.0f, 40.0f, 0.0f));

	//posses player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Forward", this, &AMainCharacter::Forward);
	InputComponent->BindAxis("Strafe", this, &AMainCharacter::Strafe);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::StopFire);

	InputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMainCharacter::StartThrow);
	InputComponent->BindAction("ThrowGrenade", IE_Released, this, &AMainCharacter::EndThrow);

	InputComponent->BindAxis("Pitch", this, &AMainCharacter::Pitch);
	InputComponent->BindAxis("Yaw", this, &AMainCharacter::Yaw);
}

void AMainCharacter::Forward(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Forward"));
	AddMovementInput(GetActorForwardVector() * Value);
}

void AMainCharacter::Strafe(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Strafe"));
	AddMovementInput(GetActorRightVector() * Value);
}

void AMainCharacter::StartCrouch()
{
}

void AMainCharacter::StopCrouch()
{
}

void AMainCharacter::Sprint()
{
	//UE_LOG(LogTemp, Warning, TEXT("Running"));
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
}

void AMainCharacter::StopSprint()
{
	//UE_LOG(LogTemp, Warning, TEXT("Not Running"));
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AMainCharacter::Yaw(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Yaw"));
	AddControllerYawInput(Value * TurnSpeed * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Pitch(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Pitch"));
	AddControllerPitchInput(Value * TurnSpeed * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::StartFire()
{
	Fire();

	GetWorldTimerManager().SetTimer(TimerHandle_Handlefire, this, &AMainCharacter::Fire, 0.08f, true);
}

void AMainCharacter::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Handlefire);
}

void AMainCharacter::Fire()
{
	FHitResult Hit;

	FVector Start = RayFire->GetComponentLocation();
	FVector ForwardVector = Camera->GetForwardVector();

	Start = Start + (ForwardVector * SpringArm->TargetArmLength / 2);
	FVector End = Start + (ForwardVector * 5000.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1, 0, 1);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);

	if (IsHit)
	{
		AActor* HitActor = Hit.GetActor();

		UGameplayStatics::ApplyPointDamage(HitActor, Damage, Start, Hit, DamageInstigator, this, DamageType);

		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(10, 10, 10), FColor::Yellow, false, 2.0f);
	}
}

void AMainCharacter::StartThrow()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grenade Pressed"));
}

void AMainCharacter::EndThrow()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grenade Released"));

	FHitResult GHit;

	FVector SpawnLocation = GrenadeSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = GrenadeSpawnPoint->GetComponentRotation();
	FVector ForwardVector = Camera->GetForwardVector();

	FVector MaxDistance = SpawnLocation + (ForwardVector * 1000.0f);

	SpawnLocation = SpawnLocation + (ForwardVector * SpringArm->TargetArmLength / 2);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	DrawDebugLine(GetWorld(), SpawnLocation, MaxDistance, FColor::Red, false, 1, 0, 1);

	bool IsGHit = GetWorld()->LineTraceSingleByChannel(GHit, SpawnLocation, MaxDistance, ECC_Visibility, CollisionParams);

	if (IsGHit)
	{
		AActor* HitActor = GHit.GetActor();

		UGameplayStatics::ApplyPointDamage(HitActor, GrenadeDamage, SpawnLocation, GHit, DamageInstigator, this, DamageType);

		DrawDebugBox(GetWorld(), GHit.ImpactPoint, FVector(10, 10, 10), FColor::Yellow, false, 2.0f);
	}

	if (Grenade) { //checks grenade projectile has been set in the editor

		AGrenade* TempGrenade = GetWorld()->SpawnActor<AGrenade>(Grenade, SpawnLocation, SpawnRotation);
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Health: "), Health);

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dead"));
	}
	return DamageAmount;
}