// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "pickUp.h"
#include "meleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class STARTERPROJECT_API AmeleeWeapon : public ApickUp
{
	GENERATED_BODY()

	AmeleeWeapon();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void onOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) override;

	UFUNCTION()
	virtual void BeginPlay();

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	//USphereComponent  *baseCollisionComponent;
	

	FTimerHandle inLineTimer;
public:
	void enableSelfDestruction(float timer);
	
private:
	void selfDestroy();
};
