// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Character Mesh"));
	//CharacterMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	//SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Mesh"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Camera->SetRelativeLocation(FVector(-300.0f, 0.0f, 200.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(DeltaRotation, true);
	AddActorLocalOffset(DeltaLocation, true);

}

void APlayerCharacter::SetMoveAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Forward & Back Value: %f"), Value);
	DeltaLocation = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.0f, 0.0f);
}

void APlayerCharacter::SetStrafeAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Strafe Value: %f"), Value);
	DeltaLocation += FVector(0.0f, Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.0f);
}

void APlayerCharacter::SetYawAmount(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Rotate Value: %f"), Value);
	float YawAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Yaw = FRotator(0.0f, YawAmount, 0.0f);
	DeltaRotation = FQuat(Yaw);
}

void APlayerCharacter::SetPitchAmount(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Yaw Value: %f"), Value);
	float PitchAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Pitch = FRotator(PitchAmount, 0.0f, 0.0f);
	DeltaRotation += FQuat(Pitch);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward & Back", this, &APlayerCharacter::SetMoveAmount);
	PlayerInputComponent->BindAxis("Yaw", this, &APlayerCharacter::SetYawAmount);
	PlayerInputComponent->BindAxis("Pitch", this, &APlayerCharacter::SetPitchAmount);
	PlayerInputComponent->BindAxis("Strafe", this, &APlayerCharacter::SetStrafeAmount);
}