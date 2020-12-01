// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetWalkSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BaseAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_SetWalkSpeed::UBTTask_SetWalkSpeed()
{
	// Set name of BTService
	NodeName = TEXT("Set Walk Speed");
}

EBTNodeResult::Type UBTTask_SetWalkSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	ABaseAICharacter* OwnerPawn = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (OwnerPawn != nullptr)
	{
		OwnerPawn->GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
	}
	return EBTNodeResult::Succeeded;
}
