// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	// Reference for AIController
	AAIController* Controller = OwnerComp.GetAIOwner();

	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;

	}

	// Refernce to AShooterCharacter that owns this AIController->Blackboard->BlackboardTask
	AShooterCharacter* Owner = Cast<AShooterCharacter>(Controller->GetPawn());

	if (Owner == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	// Call Shoot function on AShooterCharacter
	Owner->Shoot();

	return EBTNodeResult::Succeeded;
	
}

