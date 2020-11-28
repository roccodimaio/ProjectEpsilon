// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
	
	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerceptionComponent;

	/*** FUNCTIONS ***/
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void ProcessPerceivedInformation(AActor* Actor, FAIStimulus Stimulus);

	bool IsDead() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere)
	class UAISenseConfig_Hearing* HearingConfig;

	/** Reference to AIBehaviorTree */
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere)
	APawn* EnemyPawn;

	/*** FUNCTIONS ***/
	UFUNCTION(BlueprintPure)
	APawn* GetEnemyPawn();
	
};
