// Fill out your copyright notice in the Description page of Project Settings.

#include "starterProject.h"
#include "BasicAiController.h"
#include "BasicAI.h"
#include "starterProjectCharacter.h"


ABasicAiController::ABasicAiController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourComp"));

	moving = false;
	foundHim = false;
}

void ABasicAiController::Possess(class APawn *InPawn)
{
	Super::Possess(InPawn);
	ABasicAI* basicAI = Cast<ABasicAI>(InPawn);
	BestDistance = MAX_FLT; // Huge number. Saves memory instead of writting eg 10000000
	if (basicAI && basicAI->AiBehavior)
	{
		BlackboardComp->InitializeBlackboard(*basicAI->AiBehavior->BlackboardAsset);
		TargetKeyID = BlackboardComp->GetKeyID("Enemy");
		TargetLocationID = BlackboardComp->GetKeyID("Destination");

		BehaviorComp->StartTree(*(basicAI->AiBehavior));
	}

}

void ABasicAiController::SearchForTarget()
{
	APawn*MyAi = GetPawn();
	if (MyAi == NULL)
	{
		return;
	}

	const FVector Myloc = MyAi->GetActorLocation();

	AstarterProjectCharacter* TargetPawn = NULL;

	for (FConstPawnIterator I = GetWorld()->GetPawnIterator(); I; ++I)
	{
		AstarterProjectCharacter *TestPawn = Cast<AstarterProjectCharacter>(*I);

		if (TestPawn)
		{
			const float DistBetween = FVector::Dist(TestPawn->GetActorLocation(), Myloc);
			if (DistBetween < BestDistance)
			{
				BestDistance = DistBetween;
				TargetPawn = TestPawn;
				foundHim = true;
			}


		}
	}

	if (TargetPawn)
	{
		setTarget(TargetPawn);
	}
}

void ABasicAiController::setTarget(class APawn *InPawn)
{
	BlackboardComp->SetValueAsObject(TargetKeyID, InPawn);
	BlackboardComp->SetValueAsVector(TargetLocationID, InPawn->GetActorLocation());
}