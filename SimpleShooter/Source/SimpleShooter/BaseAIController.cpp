// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAICharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"


ABaseAIController::ABaseAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// Set sight as the dominant sense
	//GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//GetPerceptionComponent()->ConfigureSense(*SightConfig); 
}

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
	//AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::ProcessPerceivedInformation);
	
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::ProcessPerceivedInformation);

	// Reference to pawn that BaseAIController is in control of
	APawn* PawnControlled = GetPawn();

	if (AIBehavior != nullptr)
	{
		// Reference to Player0
		EnemyPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		RunBehaviorTree(AIBehavior);

		// Check blackboard for any keys with the name "Enemy" and key type of Object
		// Set the value of the key found to EnemyPawn
		GetBlackboardComponent()->SetValueAsObject(TEXT("Enemy"), EnemyPawn);

		// Will check the blackboard for any Keys with the name "PlayerLocation" and key type of Vector
		// Will set the value of the key with to PlayerPawn->GetActorLocation()
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), EnemyPawn->GetActorLocation());

		// Will check the blackboard for any Keys with the name "StartLocation" and key type of Vector
		// Will set the value of the key with to PawnControlled->GetActorLocation()
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), PawnControlled->GetActorLocation());
	}
}

void ABaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

APawn* ABaseAIController::GetEnemyPawn()
{
	return EnemyPawn;
}

void ABaseAIController::ProcessPerceivedInformation(AActor* Actor, FAIStimulus Stimulus)
{
	/**
	UE_LOG(LogTemp, Warning, TEXT("BaseAIController->ProcessPerceivedInformation"));

	auto TempPerceptionComponent = GetPerceptionComponent();

	if (TempPerceptionComponent != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BaseAIController->ProcessPerceivedInformation->IF TempPerceptionComponent"));

		for (auto SensedActor : UpdatedActors)
		{
			// If the sense that detected is no Sight then return
			if (!SightConfig->GetSenseID().IsValid()) return; 
			
			//UE_LOG(LogTemp, Warning, TEXT("BaseAIController->ProcessPerceivedInformation->FOR SensedActor"));

			// If the actor sensed is the Enemy set blackboard key "IsPlayerVisible"
			if (SensedActor == GetEnemyPawn())
			{
				GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerVisible"), true);
			}
			else
			{
				GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerVisible"), false);
			}
			
		}

	}


	//AIPerceptionComponent->GetActorsPerception(UpdatedActors[i], FActorPerceptionBlueprintInfo::LastSensedStimuli);

	*/
}

bool ABaseAIController::IsDead() const
{
	// Check if pawn controlled by this AI is a BaseAICharacter
	ABaseAICharacter* ControlledCharacter = Cast<ABaseAICharacter>(GetPawn());

	if (ControlledCharacter != nullptr)
	{
		return ControlledCharacter->IsDead(); 
	}

	return true;
}


