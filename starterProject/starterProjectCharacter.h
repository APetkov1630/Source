// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "pickUp.h"
#include "meleeWeapon.h"
#include "starterProjectCharacter.generated.h"

USTRUCT()
struct FAttackProps
{
	GENERATED_USTRUCT_BODY()

	float animationPlaying;
	FString animationTitle;
	float comboTimer;
	bool sheath;
	int swordComboCount;
};

UCLASS(config=Game)
class AstarterProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AstarterProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/*Animation Collection*/
	UPROPERTY(EditDefaultsOnly, Category = animationCollection)
	UAnimMontage* leftJab;

	UPROPERTY(EditDefaultsOnly, Category = animationCollection)
	UAnimMontage* rightJab;

	UPROPERTY(EditDefaultsOnly, Category = animationCollection)
	UAnimMontage* pickUP;

	UPROPERTY(EditDefaultsOnly, Category = animationCollection)
	UAnimMontage* roundHouse;

	UPROPERTY(EditDefaultsOnly, Category = animationCollection)
	UAnimMontage* firstSwordSlash;
	/*Animation Collection*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent *cLeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent *cRightHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent *cLeftShin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	class USphereComponent *cRightShin;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<class AmeleeWeapon>Katana;

	void setWeaponPointer(AmeleeWeapon*collidedWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	bool equiped;

	class AmeleeWeapon*katanaPointer;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void handleAttacks();
	void handleSwordAttacks();

	uint8 leftClickCount;
	uint8 previouseCount;
	float comboCD;

	FAttackProps attackProperties;

	FTimerHandle inLineTimer;
	FTimerHandle secondInLineTimer;
	
	void Tick(float DeltaTime);

	UFUNCTION()
	virtual void handHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void shinHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


	void sheathWeapon();
	void equipWeapon();

	bool freeze;
	void setFreeze(bool bswitch);
	void disableFreeze();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

