// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_MeleeAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "MeleeAICharacter.h"


UBTTaskNode_MeleeAttack::UBTTaskNode_MeleeAttack()
{
	// Set name of BTTask
	NodeName = TEXT("MeleeAttack");

}

EBTNodeResult::Type UBTTaskNode_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	// Get reference to AIController assocated with this Blackboard task, then get the Owning Pawn
	// Cast Owning Pawn to AMeleeAICharacter
	OwnerPawn = Cast<AMeleeAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	// Call function within OwnerPawn
	if (OwnerPawn)
	{
		// TODO Call AttackMontage

		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}
