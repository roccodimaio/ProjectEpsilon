// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		// Reference to Player
		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		// Seet the value of Blackboard key named StartLocation
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); 
	}
	
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}
