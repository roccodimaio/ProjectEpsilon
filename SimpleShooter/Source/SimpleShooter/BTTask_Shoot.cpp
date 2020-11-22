// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "AIController.h"


UBTTask_Shoot::UBTTask_Shoot()
{
	// Set name of BTTask
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	// Get reference to AIController assocated with this Blackboard task, then get the Owning Pawn
	// Cast Owning Pawn to AShooterCharacter
	OwnerPawn = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	// Get reference to AIController assocated with this Blackboard task
	if (OwnerPawn)
	{
		OwnerPawn->PullTrigger();
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
	

