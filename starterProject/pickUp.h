// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "pickUp.generated.h"

UCLASS()
class STARTERPROJECT_API ApickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ApickUp();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	/** The collision object for the actor. **/
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	//USphereComponent  *baseCollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	USkeletalMeshComponent *pickupMesh;

	UFUNCTION()
	virtual void onOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
};
