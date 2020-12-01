// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindWanderLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAICharacter.h"
#include "AIController.h"

UBTTask_FindWanderLocation::UBTTask_FindWanderLocation()
{
	// Set name of BTService
	NodeName = TEXT("Find Wander Location");
}

EBTNodeResult::Type UBTTask_FindWanderLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	if (GetWorld())
	{
		
		UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

		if (NavigationArea)
		{
			// Get reference to AIController assocated with this Blackboard task, then get the Owning Pawn
			// Cast Owning Pawn to AShooterCharacter
			OwnerPawn = Cast<ABaseAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
			
			// Get current position of owning actor 
			OriginVector = OwnerPawn->GetActorLocation();

			// Find random point within SearchRadius from current position of owning actor
			RandomLocationVector = NavigationArea->GetRandomReachablePointInRadius(this, OriginVector, SearchRadius);

			//UE_LOG(LogTemp, Warning, TEXT("FindWanderLocation->RandomLocationVector is %s"), *RandomLocationVector.ToString());

			// Set value of the blackboard key associated with this BTService. Blackboard key selected in Bluprint
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocationVector);
		}
	}
	return EBTNodeResult::Succeeded;
}
