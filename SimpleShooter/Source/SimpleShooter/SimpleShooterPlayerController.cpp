// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

ASimpleShooterPlayerController::ASimpleShooterPlayerController()
{

}

void ASimpleShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	UE_LOG(LogTemp, Warning, TEXT("ASimpleShooterPlayerController -> GameHasEnded"));

	if (bIsWinner)
	{
		// Create LoseScreen widget
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);

		if (WinScreen != nullptr)
		{
			// Display LoseScreen widget
			WinScreen->AddToViewport();
		}
	}
	else
	{
		// Create LoseScreen widget
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);

		if (LoseScreen != nullptr)
		{
			// Display LoseScreen widget
			LoseScreen->AddToViewport();
		}
	}
	
	// Set timer to call the function RestartLevel from APlayerController
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);

}
