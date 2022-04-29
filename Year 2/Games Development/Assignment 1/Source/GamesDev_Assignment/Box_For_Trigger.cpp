// Fill out your copyright notice in the Description page of Project Settings.


#include "Box_For_Trigger.h"
#include "Components/BoxComponent.h"

// Sets default values
ABox_For_Trigger::ABox_For_Trigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box Collision Mesh"));
	SetRootComponent(BoxTriggerMesh);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CollisionBox->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	CollisionBox->SetCollisionProfileName("Trigger");
	CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CollisionBox->SetupAttachment(BoxTriggerMesh);
}

// Called when the game starts or when spawned
void ABox_For_Trigger::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABox_For_Trigger::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ABox_For_Trigger::OnOverlapEnd);
}

// Called every frame
void ABox_For_Trigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABox_For_Trigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Entered"));
}

void ABox_For_Trigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exited"));
}

