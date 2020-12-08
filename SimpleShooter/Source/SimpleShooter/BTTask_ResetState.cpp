// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetState.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTTask_ResetState::UBTTask_ResetState()
{
	// Set name of BTService
	NodeName = TEXT("Reset State");
}

EBTNodeResult::Type UBTTask_ResetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	ABaseAIController* OwnerController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (OwnerController != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), 0);
	}

	return EBTNodeResult::Type();
}
