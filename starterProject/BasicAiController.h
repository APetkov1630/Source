 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BasicAI.h"
#include "BasicAiController.generated.h"

/**
 * 
 */
UCLASS()
class STARTERPROJECT_API ABasicAiController : public AAIController
{
	GENERATED_BODY()
public:
	ABasicAiController();

	UPROPERTY(transient)
	class UBlackboardComponent * BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent *BehaviorComp;

	virtual void Possess(class APawn *InPawn);

	void setTarget(class APawn *InPawn);

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void SearchForTarget();


protected:
	uint8 TargetKeyID;       //Using Uint saves memory. Name of the target im looking for 
	uint8 TargetLocationID;  //Target Location 
	uint8 RandomLocationID;
	float BestDistance;
	bool foundHim;          //True when ai finds player
	bool moving;             //Checks if his moving
	FVector RandomLocaiton;

};
