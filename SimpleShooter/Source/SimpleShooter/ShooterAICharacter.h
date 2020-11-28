// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "ShooterAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAICharacter : public ABaseAICharacter
{
	GENERATED_BODY()
	
public:

	AShooterAICharacter();

protected: 
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** Function to shoot equipped Gun.cpp */
	void PullTrigger();

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;
	
	UPROPERTY()
	AGun* Gun = nullptr;
};
