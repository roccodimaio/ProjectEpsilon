// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "MeleeCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"

AMeleeAIController::AMeleeAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	AIPerceptionComponent->ConfigureSense(*SightConfig); 
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMeleeAIController::ProcessPerceivedInformation);

}

void AMeleeAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);

		// Reference to Player0
		EnemyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		// Reference to Pawn that MeleeAIController is in control of
		APawn* PawnControlled = GetPawn(); 

		// Check blackboard for any keys with the name "Enemy" and key type of Object
		// Set the value of the key found to EnemyPawn
		GetBlackboardComponent()->SetValueAsObject(TEXT("Enemy"), EnemyPawn);
	}

}

void AMeleeAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AMeleeAIController::IsDead() const
{
	// Check if pawn controlled by this AI is a MeleeCharacter
	AMeleeCharacter* ControlledCharacter = Cast<AMeleeCharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead(); 
	}
	
	return true;
}

void AMeleeAIController::ProcessPerceivedInformation(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("MeleeAIController->ProcessPerceivedInformation")); 
}

APawn* AMeleeAIController::GetEnemyPawn()
{
	return EnemyPawn;
}
