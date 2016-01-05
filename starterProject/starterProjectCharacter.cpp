// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "starterProject.h"
#include "Engine.h"
#include "BasicAI.h"
#include "starterProjectCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AstarterProjectCharacter

AstarterProjectCharacter::AstarterProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bDoCollisionTest = false;
		
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	cLeftHand = CreateDefaultSubobject<USphereComponent>(TEXT("Left Hand Collision"));
	cLeftHand->InitSphereRadius(25.0f);
	cLeftHand->OnComponentBeginOverlap.AddDynamic(this, &AstarterProjectCharacter::handHit);
	cLeftHand->AttachTo(Mesh, "LeftHand");

	cRightHand = CreateDefaultSubobject<USphereComponent>(TEXT("Right Hand Collision"));
	cRightHand->InitSphereRadius(25.0f);
	cRightHand->OnComponentBeginOverlap.AddDynamic(this, &AstarterProjectCharacter::handHit);
	cRightHand->AttachTo(Mesh, "RightHand");

	cLeftShin = CreateDefaultSubobject<USphereComponent>(TEXT("Left Shin Collision"));
	cLeftShin->InitSphereRadius(20.0f);
	cLeftShin->OnComponentBeginOverlap.AddDynamic(this, &AstarterProjectCharacter::shinHit);
	cLeftShin->AttachTo(Mesh, "LeftFoot");

	cRightShin = CreateDefaultSubobject<USphereComponent>(TEXT("Right Shin Collision"));
	cRightShin->InitSphereRadius(20.0f);
	cRightShin->OnComponentBeginOverlap.AddDynamic(this, &AstarterProjectCharacter::shinHit);
	cRightShin->AttachTo(Mesh, "RightFoot");

	comboCD = 1.30;
	attackProperties.comboTimer = 0.0;
	leftClickCount = 0;
	previouseCount = 0;
	freeze = false;
	attackProperties.sheath = false;
	equiped = false;

	attackProperties.swordComboCount = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AstarterProjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("leftClick", IE_Pressed, this, &AstarterProjectCharacter::handleAttacks);
	InputComponent->BindAction("Num2", IE_Pressed, this, &AstarterProjectCharacter::equipWeapon);

	InputComponent->BindAxis("MoveForward", this, &AstarterProjectCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AstarterProjectCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AstarterProjectCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AstarterProjectCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AstarterProjectCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AstarterProjectCharacter::TouchStopped);
}


void AstarterProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AstarterProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AstarterProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AstarterProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AstarterProjectCharacter::MoveForward(float Value)
{
	if (!freeze)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
	}
}

void AstarterProjectCharacter::MoveRight(float Value)
{
	if (!freeze)
	{
		if ((Controller != NULL) && (Value != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

void AstarterProjectCharacter::setWeaponPointer(AmeleeWeapon*collidedWeapon)
{
	if (attackProperties.animationPlaying == 0)
	{
		this->Katana = collidedWeapon->GetClass();
		this->setFreeze(true);
		this->PlayAnimMontage(pickUP);
		attackProperties.animationPlaying = this->PlayAnimMontage(pickUP);
		GetWorld()->GetTimerManager().SetTimer(inLineTimer, this, &AstarterProjectCharacter::disableFreeze, attackProperties.animationPlaying, false);
		GetWorld()->GetTimerManager().SetTimer(secondInLineTimer, this, &AstarterProjectCharacter::sheathWeapon, attackProperties.animationPlaying, false);
		collidedWeapon->enableSelfDestruction(attackProperties.animationPlaying - 1.0f);//staticWeapon
	}
}

void AstarterProjectCharacter::sheathWeapon()
{
	if (Katana != NULL)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = this;
		SpawnParam.Instigator = Instigator;
		katanaPointer = GetWorld()->SpawnActor<AmeleeWeapon>(Katana, SpawnParam);
		katanaPointer->AttachRootComponentTo(Mesh, "staticWeapon", EAttachLocation::SnapToTarget);
		attackProperties.sheath = true;
	}
}

void AstarterProjectCharacter::equipWeapon()
{
	if (katanaPointer != NULL)
	{
		if (attackProperties.sheath)
		{
			katanaPointer->pickupMesh->SetWorldScale3D(FVector(5.0, 5.0, 5.0));
			katanaPointer->AttachRootComponentTo(Mesh, "RightWeaponSocket", EAttachLocation::SnapToTarget);
			equiped = true;
			attackProperties.sheath = false;
		}
		else if (!attackProperties.sheath)
		{
			katanaPointer->AttachRootComponentTo(Mesh, "staticWeapon", EAttachLocation::SnapToTarget);
			katanaPointer->pickupMesh->SetWorldScale3D(FVector(5.0, 5.0, 5.0));
			attackProperties.sheath = true;
			equiped = false;
		}
	}
}

void AstarterProjectCharacter::disableFreeze()
{
	this->setFreeze(false);
}

void AstarterProjectCharacter::setFreeze(bool bswitch)
{
	freeze = bswitch;
}
void AstarterProjectCharacter::handleAttacks()
{
	if (!freeze)
	{
		if (!equiped)
		{
			switch (leftClickCount)
			{
			case 0:
			{
				if (attackProperties.animationPlaying == 0)
				{
					this->PlayAnimMontage(leftJab);
					attackProperties.animationPlaying = this->PlayAnimMontage(leftJab);
					attackProperties.animationPlaying -= 0.37;
					attackProperties.animationTitle = "Punching";
					leftClickCount++;
					attackProperties.comboTimer = 0.0f;
				}
				else {
					return;
				}
			}
			break;

			case 1:
			{
				if (attackProperties.animationPlaying == 0)
				{
					this->PlayAnimMontage(rightJab);
					attackProperties.animationPlaying = this->PlayAnimMontage(rightJab);
					attackProperties.animationTitle = "Punching";
					attackProperties.animationPlaying -= 0.37;
					leftClickCount++;
				}
				else {
					return;
				}
			}
			break;

			case 2:
			{
				if (attackProperties.animationPlaying == 0)
				{
					this->PlayAnimMontage(roundHouse);
					attackProperties.animationPlaying = this->PlayAnimMontage(roundHouse);
					attackProperties.animationPlaying -= 0.37;
					attackProperties.animationTitle = "Kicking";
					leftClickCount = 0;
				}
				else {
					return;
				}
			}
			break;
			}
		} else {
			handleSwordAttacks();
		}
	}
	
}

void AstarterProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (attackProperties.comboTimer < comboCD)
	{
		attackProperties.comboTimer += DeltaTime;
	}

	if (attackProperties.animationPlaying > 0)
	{
		attackProperties.animationPlaying -= DeltaTime;
	}
	else { attackProperties.animationPlaying = 0.0f; }
}

void AstarterProjectCharacter::handHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AstarterProjectCharacter*Self = Cast<AstarterProjectCharacter>(OtherActor);
	ABasicAI*enemy = Cast<ABasicAI>(OtherActor);
	if (!Self)
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		if (attackProperties.animationTitle.Equals("Punching", ESearchCase::IgnoreCase) && attackProperties.animationPlaying > 0)
		{
			if (enemy)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, SweepResult.BoneName.ToString());
			}
			
		}
	}
}

void AstarterProjectCharacter::shinHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AstarterProjectCharacter*Self = Cast<AstarterProjectCharacter>(OtherActor);

	if (!Self)
	{
		if (attackProperties.animationTitle.Equals("Kicking", ESearchCase::IgnoreCase) && attackProperties.animationPlaying > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Hit Leg")));
		}
	}
}

void AstarterProjectCharacter::handleSwordAttacks()
{

	switch (attackProperties.swordComboCount)
	{
	case 0:
	{
		if (attackProperties.animationPlaying == 0)
		{
			this->PlayAnimMontage(firstSwordSlash);
			attackProperties.animationPlaying = this->PlayAnimMontage(firstSwordSlash);
			attackProperties.animationPlaying -= 0.37;
			attackProperties.animationTitle = "Sword";
			//attackProperties.swordComboCount++;
			//attackProperties.comboTimer = 0.0f;
		}
		else {
			return;
		}
	}
	break;
	}
	/*	case 1:
		{
			if (attackProperties.animationPlaying == 0)
			{
				this->PlayAnimMontage(rightJab);
				attackProperties.animationPlaying = this->PlayAnimMontage(rightJab);
				attackProperties.animationTitle = "Punching";
				attackProperties.animationPlaying -= 0.37;
				leftClickCount++;
			}
			else {
				return;
			}
		}
		break;

		case 2:
		{
			if (attackProperties.animationPlaying == 0)
			{
				this->PlayAnimMontage(roundHouse);
				attackProperties.animationPlaying = this->PlayAnimMontage(roundHouse);
				attackProperties.animationPlaying -= 0.37;
				attackProperties.animationTitle = "Kicking";
				leftClickCount = 0;
			}
			else {
				return;
			}
		}
		break;
		}*/
}