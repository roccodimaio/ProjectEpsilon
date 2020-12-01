// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterAICharacter.h"
#include "AIController.h"

UBTT_FocusTarget::UBTT_FocusTarget()
{
	// Set name of BTTask
	NodeName = TEXT("Focus On Target");
}

EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	// Store value of blackboard key Enemy to FocusActor if Enemy is an AActor
	AActor* FocusActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Enemy"));

	if (FocusActor != nullptr)
	{
		// Tell owner to focus on FocusActor
		OwnerComp.GetAIOwner()->SetFocus(FocusActor);
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;

}
