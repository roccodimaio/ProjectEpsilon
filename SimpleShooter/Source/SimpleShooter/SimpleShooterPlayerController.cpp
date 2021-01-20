// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "BaseWeapon.h"
#include "BaseWeapon_Gun.h"
#include "BaseWeapon_Gun_Projectile.h"


ASimpleShooterPlayerController::ASimpleShooterPlayerController()
{

}

void ASimpleShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Create LoseScreen widget
	HUD = CreateWidget(this, HUDClass);
	//GunHUD = CreateWidget(this, GunHUDClass);
	if (EquippedWeaponHUDClass != nullptr)
	{
		EquippedWeaponHUD = CreateWidget(this, EquippedWeaponHUDClass);
	}
	

	if (HUD != nullptr)
	{
		// Display LoseScreen widget
		HUD->AddToViewport();
	}

}

void ASimpleShooterPlayerController::DisplayWeaponHUD(ABaseWeapon* Weapon)
{
	if (Weapon != nullptr)
	{
		ABaseWeapon_Gun_Projectile* EquippedWeapon = Cast<ABaseWeapon_Gun_Projectile>(Weapon);
		//ABaseWeapon* EquippedWeapon = Cast<ABaseWeapon>(Weapon);

		if (EquippedWeapon)
		{
			if (EquippedWeaponHUD != nullptr)
			{
				EquippedWeaponHUD->AddToViewport();
			}
		}
		else
		{
			if (EquippedWeaponHUD != nullptr)
			{
				EquippedWeaponHUD->RemoveFromViewport();
			}
		}
	}
	else
	{
		if (EquippedWeaponHUD != nullptr)
		{
			EquippedWeaponHUD->RemoveFromViewport();
		}
	}
}

void ASimpleShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);
	//UE_LOG(LogTemp, Warning, TEXT("ASimpleShooterPlayerController -> GameHasEnded"));
	
	// Remove HUD from Viewport once game is over (dead or win)
	HUD->RemoveFromViewport();

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
