// Fill out your copyright notice in the Description page of Project Settings.


#include "TeaPawn.h"

// Sets default values
ATeaPawn::ATeaPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pawn Mesh"));
	PawnMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PawnMesh);

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Mesh"));
	Camera->SetupAttachment(SpringArm);

	Camera->SetRelativeLocation(FVector(-300.0f, 0.0f, 200.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(PawnMesh);
	ProjectileSpawnPoint->SetRelativeLocation(FVector(160.0f, 0.0f, 140.0f));


	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ATeaPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATeaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(DeltaRotation, true);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATeaPawn::SetMoveAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Drive Value: %f"), Value);
	DeltaLocation = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f);
}

void ATeaPawn::SetStrafeAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Drive Value: %f"), Value);
	DeltaLocation += FVector(0.0f, Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.0f);
}

void ATeaPawn::SetRotateAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Drive Value: %f"), Value);
	float RotateAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0.0f, RotateAmount, 0.0f);
	DeltaRotation = FQuat(Rotation);
}

void ATeaPawn::OnBeginFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Pressed"));
}

void ATeaPawn::OnEndFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Released"));

	if (TeaBagClass) { //checks teabag projectile has been set in the editor
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		ATeaBag* TempBag = GetWorld()->SpawnActor<ATeaBag>(TeaBagClass, SpawnLocation, SpawnRotation);
	}

}

// Called to bind functionality to input
void ATeaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

		PlayerInputComponent->BindAxis("Drive", this, &ATeaPawn::SetMoveAmount);
PlayerInputComponent->BindAxis("Turn", this, &ATeaPawn::SetRotateAmount);
	PlayerInputComponent->BindAxis("Strafe", this, &ATeaPawn::SetStrafeAmount);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATeaPawn::OnBeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATeaPawn::OnEndFire);
}