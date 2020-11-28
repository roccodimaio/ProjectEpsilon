// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*** FUNCTIONS ***/

	/** Function to take damage */
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Function to check if AShooterCharacter is dead */
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Function to return current health percentage */
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	/** Function to return current health */
	UFUNCTION(BlueprintPure)
	float GetCurrentHealth() const;

	/** Function to return max health */
	UFUNCTION(BlueprintPure)
	float GetMaxHealth() const;

	/*** VARIABLES ***/
	
	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackVocalSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AttackSwingSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticleSystem;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
};
