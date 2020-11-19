// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"

AShooterAIController::AShooterAIController()
{

}

void AShooterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Assign PlayerCharacter to PlayerPawn (refence to player)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	
	if (LineOfSightTo(PlayerPawn))
	{
		SetFocus(PlayerPawn);
		MoveToActor(PlayerPawn, MoveToRadius);
	}
	else
	{
		ClearFocus(EAIFocusPriority::Gameplay);
		StopMovement();
	}
	
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay(); 

}
 