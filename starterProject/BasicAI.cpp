// Fill out your copyright notice in the Description page of Project Settings.

#include "starterProject.h"
#include "BasicAI.h"
#include "Engine.h"
#include "BasicAiController.h"
#include "Animation/AnimInstance.h"


ABasicAI::ABasicAI()
{
	AIControllerClass = ABasicAiController::StaticClass();

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickUp CollisionComp"));
	CollisionComp->SetSphereRadius(100.0f);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABasicAI::onCollision);
	CollisionComp->AttachTo(RootComponent);

	//bloodEffect = PCIP.CreateDefaultSubobject < UParticleSystemComponent >(this, TEXT("IceEffect"));
	//bloodEffect->AttachTo(RootComponent);
	//bloodEffect->Deactivate();

	moveSpeed = 500.0f;
	CharacterMovement->MaxWalkSpeed = moveSpeed;
	damage = 20;

	isColliding = false;
	shouldAttack = true;
	attackTimer = 0.0f;
	burnTimer = 0.0f;
	Fire = false;
	burnDuration = 0.0f;
	initialDamage = 0.0f;
	burnDamage = 0.0f;
	stillBurning = false;
	attackOffset = 1.0f;
	ice = false;
	isStunned = false;
	worth = 60;
	maxHealth = 500;

	slowDuration = 0.0f; //Will be initlized in icehit
}

void ABasicAI::BeginPlay()
{
	Super::BeginPlay();


	health = maxHealth;
}
void ABasicAI::takeDamage(float damage)
{
	if (health >= 1)
	{
		float scaleFactor = damage / 10; //will set the scale to be 10 percent of the damage. This way the mroe damage a spell does the more blood.
		health -= damage;
		//bloodEffect->Activate();
		//bloodEffect->SetWorldScale3D(FVector(scaleFactor, scaleFactor, scaleFactor));

	}

}

bool ABasicAI::shouldDestroy()
{
	if (health <= 0)
		return true;


	return false;
}

bool ABasicAI::timerOffset(float time, float offSet, bool var)
{
	attackTimer += time;
	float Offset = offSet;


	if (attackTimer >= Offset&&!var)
	{
		var = true;
		attackTimer -= Offset;
		return var;

	}
	else
	{
		return var;
	}


}

void ABasicAI::checkAttack(float DeltaTime)
{

	if (isColliding && timerOffset(DeltaTime, attackOffset, shouldAttack && !isStunned))  // Timmer offSet will 
	{
		/*Player->takeDamage(damage);*/

		if (attacking != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(attacking, 1.0f);
			}
		}

		shouldAttack = false;
	}
}

void ABasicAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	burnAI(DeltaTime);
	slowDecay(DeltaTime);
	checkAttack(DeltaTime);


}

#pragma region Collision
void ABasicAI::onCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	/*AMyProject2Character*Player = Cast<AMyProject2Character>(OtherActor);

	if (Player)
	{
		this->Player = Player;

		isColliding = true;


	}*/
}

void ABasicAI::endCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	isColliding = false;


}
#pragma endregion

#pragma region Spell Effects

#pragma region BurnEffect
void ABasicAI::Initilizeburn(float initialDamage, float bDamage, float burnDuration)
{

	float bDuration = burnDuration;
	this->burnDuration = burnDuration;
	this->initialDamage = initialDamage;
	burnDamage = bDamage;

	if (!stillBurning)
	{
		takeDamage(initialDamage);
		Fire = true;
	}
	else
	{
		stillBurning = false;
	}


}

void ABasicAI::burnAI(float DeltaTime)
{
	if (Fire)
	{
		burnTimer += DeltaTime;
		if (burnTimer <= burnDuration)
		{
			takeDamage(burnDamage);
			stillBurning = true;

		}
		else
		{
			burnTimer -= burnDuration;
			Fire = false;
			stillBurning = false;
		}
	}
}

void ABasicAI::removeFire()
{
	Fire = false;
}

bool ABasicAI::getFire()
{
	return Fire;
}
#pragma endregion

#pragma region Ice
void ABasicAI::iceHit(float Duration)
{
	ice = true;
	slowDuration = Duration;
}

bool ABasicAI::getIce()
{
	return ice;
}

void ABasicAI::removeIce()
{
	ice = false;
}


void ABasicAI::slowDecay(float DeltaTime)
{
	if (ice)
	{
		bool timeOver = false;

		if (timerOffset(DeltaTime, slowDuration, timeOver))
		{
			CharacterMovement->MaxWalkSpeed = moveSpeed;
			ice = false;
		}

	}
	else
	{
		CharacterMovement->MaxWalkSpeed = moveSpeed; // Causes bugs unless this code is here. (elese to first if statment )

	}
}
#pragma endregion

#pragma region Combo of ice and Electric
void ABasicAI::StunTarget()
{
	isStunned = true;
	CharacterMovement->MaxWalkSpeed = 0;
	CharacterMovement->bIgnoreBaseRotation = true;
	//GetWorldTimerManager().SetTimer(this, &ABasicAI::removeStun, 4.0f, false); // Unreals pre defined timer since ive used my custom made timer twice i thought for simplicity ill use this one.


}

void ABasicAI::removeStun()
{
	isStunned = false;
	CharacterMovement->MaxWalkSpeed = moveSpeed;
	CharacterMovement->bIgnoreBaseRotation = false;
}
#pragma endregion

#pragma endregion 

void ABasicAI::sendWorth()
{
	////Spawner will call this method just before destroying the AI.
	//if (Player == NULL)
	//{
	//	Player = Cast<AMyProject2Character>(UGameplayStatics::GetPlayerPawn(this, 0));
	//}
	//Player->giveGold(worth);
}

float ABasicAI::getMaxHP()
{
	return maxHealth;
}

