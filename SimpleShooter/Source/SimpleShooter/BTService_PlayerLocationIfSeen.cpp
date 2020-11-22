// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	// Set name of BTService
	NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Assign Player0 to PlayerPawn (refence to player)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr) return;

	if (OwnerComp.GetAIOwner() == nullptr) return;

	// IF LineOfSite with Player0
	if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
	{
		// Set value of the blackboard key associated with this BTService. Blackboard key selected in Bluprint
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
	}
	else
	{
		// There is no LineOfSite with Player, as a result clear the blackboard key associated with this BTService
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}


}
