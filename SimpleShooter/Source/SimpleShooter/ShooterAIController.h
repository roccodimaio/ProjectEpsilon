// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AShooterAIController();

	virtual void Tick(float DeltaSeconds) override;

	bool IsDead() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Reference to AIBehaviorTree */
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;



};