// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "ShooterCharacter.h"
#include "SimpleShooterGameModeBase.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
}

// Called every frame
void ABaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABaseAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//UE_LOG(LogTemp, Warning, TEXT("BaseAICharacter->DamageTaken"));
	
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (Health - DamageToApply <= 0.0f)
	{
		Health = 0;
		
		bIsDead = true;

		//UE_LOG(LogTemp, Warning, TEXT("BaseAICharacter->TakeDamage->Health <= 0 ->bIsDead %s"), bIsDead ? TEXT("True") : TEXT("False"));

		// Store reference to the SimpleShooterGameModeBase
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();

		if (GameMode != nullptr)
		{
			// GameMode will handle events after Character is killed (score, subtract lives, track wave numbers, etc.)
			GameMode->PawnKilled(this);
		}

		
		// Remove controller from Character.  Results in no longer being able to attack, move, etc. 
		DetachFromControllerPendingDestroy(); 

		// Turn off capsule collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); // NOT CURRENTLY WORKING

	}
	else
	{
		Health -= DamageToApply;
	}

	return DamageToApply;
}

bool ABaseAICharacter::IsDead() const
{
	return bIsDead;
}

float ABaseAICharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float ABaseAICharacter::GetCurrentHealth() const
{
	return Health;
}

float ABaseAICharacter::GetMaxHealth() const
{
	return MaxHealth;
}

