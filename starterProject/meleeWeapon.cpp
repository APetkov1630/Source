// Fill out your copyright notice in the Description page of Project Settings.

#include "starterProject.h"
#include "Engine.h"
#include "starterProjectCharacter.h"
#include "meleeWeapon.h"

AmeleeWeapon::AmeleeWeapon()
{
	pickupMesh->SetWorldScale3D(FVector(5.0, 5.0, 5.0));
	pickupMesh->OnComponentBeginOverlap.AddDynamic(this, &AmeleeWeapon::onOverlap);
	pickupMesh->SetNotifyRigidBodyCollision(true);
	
	///*baseCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("PickUp CollisionComp"));
	//baseCollisionComponent->SetSphereRadius(40);
	//RootComponent = baseCollisionComponent;*/
	//baseCollisionComponent->OnComponentHit.AddDynamic(this, &AmeleeWeapon::OnHit);
	//pickupMesh->AttachTo(RootComponent); 
	////baseCollisionComponent->SetSimulatePhysics(true);
}

void AmeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (pickupMesh != NULL)
	{
		pickupMesh->OnComponentBeginOverlap.RemoveDynamic(this, &AmeleeWeapon::onOverlap);
		pickupMesh->OnComponentBeginOverlap.AddDynamic(this, &AmeleeWeapon::onOverlap);
		pickupMesh->SetNotifyRigidBodyCollision(true);
		pickupMesh->bGenerateOverlapEvents = true;
	}
}


void AmeleeWeapon::onOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	Super::onOverlap(OtherActor, OtherComp,  OtherBodyIndex, bFromSweep, SweepResult);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, SweepResult.BoneName.ToString());
	AstarterProjectCharacter*Player = Cast<AstarterProjectCharacter>(OtherActor);

	if (Player)
	{
		Player->setWeaponPointer(this);
	}

}

void AmeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AmeleeWeapon::enableSelfDestruction(float timer)
{
	GetWorld()->GetTimerManager().SetTimer(inLineTimer, this, &AmeleeWeapon::selfDestroy, timer, false);
}

void AmeleeWeapon::selfDestroy()
{
	Destroy();
}

