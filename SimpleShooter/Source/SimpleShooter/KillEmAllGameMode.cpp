// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"
#include "MeleeAIController.h"
#include "BaseAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	//UE_LOG(LogTemp, Warning, TEXT("AKillEmAllGameMode -> PawnKilled"));

	// If a Player was killed then PlayerController will not be nullptr;
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());

	if (PlayerController != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AKillEmAllGameMode -> PawnKilled -> PLAYER KILLED"));
		EndGame(false);
	}

	// Loop through all Pawns that have AShooterAIController as their AIController
	for (ABaseAIController* Controller : TActorRange<ABaseAIController>(GetWorld()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("AKillEmAllGameMode -> PawnKilled -> ABaseAIController Loop"));
		
		// If any Pawns with ABaseAIController are NOT dead then game is NOT over
		//UE_LOG(LogTemp, Warning, TEXT("BaseAICharacter->TakeDamage->Health <= 0 ->bIsDead %s"), Controller->IsDead() ? TEXT("True") : TEXT("False"));
		
		if (!Controller->IsDead())
		{
			//UE_LOG(LogTemp, Warning, TEXT("AKillEmAllGameMode -> PawnKilled -> ABaseAIController Loop -> IF"));
			return;
		}
	}
	
	/**for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
	{
		// If any Pawns with AShooterAIController are NOT dead then game is NOT over
		if (!Controller->IsDead())
		{
			return; 
		}
	}*/

	// End game because all Pawns that have AShooterAIController are dead
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	// Return and loop through list of all AControllers within the World (Level);
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{	
		//Does Controller belong to a Player?
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner); 
		
	}
}
