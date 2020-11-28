// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AShooterAIController::AShooterAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay(); 

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		// Reference to Player0
		EnemyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		// Reference of Pawn ShooterAIController is in control of
		APawn* PawnControlled = GetPawn(); 

		// Will check the blackboard for any Keys with the name "PlayerLocation" and key type of Vector
		// Will set the value of the key with to PlayerPawn->GetActorLocation()
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), EnemyPawn->GetActorLocation());

		// Will check the blackboard for any Keys with the name "StartLocation" and key type of Vector
		// Will set the value of the key with to PawnControlled->GetActorLocation()
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), PawnControlled->GetActorLocation());

		GetBlackboardComponent()->SetValueAsObject(TEXT("Enemy"), EnemyPawn);
	}

}

APawn* AShooterAIController::GetEnemyPawn()
{
	return EnemyPawn;
}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

bool AShooterAIController::IsDead() const
{
	// Check if Pawn associated with this AI is a AShooterCharacter
	AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead();
	}
	
	return true;
}
 