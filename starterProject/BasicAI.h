// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasicAI.generated.h"

UCLASS()
class STARTERPROJECT_API ABasicAI : public ACharacter
{
	GENERATED_BODY()

public:
	ABasicAI();

	UPROPERTY(EditAnywhere, Category = Behavior)
	class UBehaviorTree* AiBehavior;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particle)
	class UParticleSystemComponent*bloodEffect;*/

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pickup)
	USphereComponent  *CollisionComp;

	void takeDamage(float damage);

	/*Max Health is needed so i can use it in blueprint to devide health bar.
	This way the healthbar will scale properly no matter the health*/
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float maxHealth;


	UFUNCTION()
	virtual void onCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);


	UFUNCTION()
	virtual void endCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "update")
	virtual void Tick(float DeltaTime);

	bool timerOffset(float time, float offSetm, bool var);

	/*Just to clear up Tick Method dont want chuncks of code inside it*/
	void checkAttack(float DeltaTime);

	/*Fire*/
	void Initilizeburn(float initialDamage, float bDamage, float burnDuration); //This will be called when the player is hit by a fire ball instead of a normal projectile.
	bool getFire();
	void removeFire();

	/** AnimMontage to play each time the ai attacks*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* attacking;

	/*ICe*/
	void iceHit(float durration);  //When hit with an iceball it will slow the Ai for a periox of time. However need to de slow the Ai in this class. Sinmce the spell gets destroyed
	void removeIce();
	bool getIce();

	/*Ice and Electric Spell Combination Effect (in electric spell ill check if any of
	the AI are slowed if so it will double the damage(in spell class) and stun them (here)*/
	void StunTarget();
	void removeStun();

	bool shouldDestroy();
	/*On death Ai will give the player gold. THis mwethod will be called by the spawner*/
	void sendWorth();

	float getMaxHP();

	UFUNCTION()
	virtual void BeginPlay();

protected:
	void burnAI(float DeltaTime);/*Since the burn effects needs to be called by the projectile when it hits the AI. I needed 2
								 methods one for initilizing the daamge and when it should be called. And one to actually do damageover tiem which needs to be called by the tick function*/

	void slowDecay(float DeltaTime); //Will remove slow after a certain amout of period

	/*Set it as visible anywhere so i can add the health widget*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
	float health;

	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float damage;
	float attackTimer;   //Holds current delta seconds. To messure when AI should attack.
	bool shouldAttack; //When 2 seconds have past the ai can attack again
	bool isColliding;  //While this is true the ai will attack the player

	/*Burn effect variables (take damage over time)*/
	float burnTimer;     //How long the projectile should burn the AI
	bool Fire;           //Boolean will get activated when burn is initilized and then Ai will take burn daamge.(origenally called should burn)
	bool stillBurning;   //Prevents me from burning the same target twice
	float burnDuration;  //How long the burn duration will last (later cna be upgraded in shop)
	float initialDamage; //The initial damage when it hits
	float burnDamage;    //How much daamge ai will take each second
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float attackOffset;  //Each Ai will have different off set (some attack faster than others)
	bool ice;            //Var will be true when hit by an ice ball.
	UPROPERTY(EditDefaultsOnly, Category = Stats)
	float moveSpeed;     /*When the Ai is slow after a certain perdio of time the movement speed needs to be set back to normal.
							 for some Ai that will be faster than other so i need a varible that will be set to max walk speed instead of just inputing it. */
	bool isStunned;      /*If this var is true then the AI cant attack. I cant just change the should attack variable
						 since after 2 seconds it will get changed to true in the timer*/
	float slowDuration;   //How long the ice spell last.
	int8 worth;          //How much money ai is worth. (since this value will never be bigger than 256 theres no points of using anything bigger)
	/*AMyProject2Character*Player;*/

};
