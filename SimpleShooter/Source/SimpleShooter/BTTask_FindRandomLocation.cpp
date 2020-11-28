// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	// Set name of BTService
	NodeName = TEXT("Find Random Location");


}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Type();
}
