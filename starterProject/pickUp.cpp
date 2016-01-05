// Fill out your copyright notice in the Description page of Project Settings.

#include "starterProject.h"
#include "pickUp.h"


// Sets default values
ApickUp::ApickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pickupMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pick Up Mesh"));
	
	//baseCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickUp CollisionComp"));
	//baseCollisionComponent->SetSphereRadius(40);

	//RootComponent = baseCollisionComponent;

	//pickupMesh->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ApickUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ApickUp::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
//
void ApickUp::onOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

}
