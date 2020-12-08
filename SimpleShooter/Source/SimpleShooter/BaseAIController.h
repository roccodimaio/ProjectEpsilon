// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */

/** ENUM that holds different Attacking states of BaseAIController */
UENUM(BlueprintType)
enum class EAttackingState : uint8
{
	EAS_Holding UMETA(DisplayName = "Holding"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Recovering UMETA(DisplayName = "Recovering"),
	EAS_MAX UMETA(DisplayName = "DefaulMax")
};

UCLASS()
class SIMPLESHOOTER_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();
	
	/*** COMPONENTS ***/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAIPerceptionComponent* AIPerceptionComponent;

	/*** VARIABLES ***/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ENUMS")
	EAttackingState AttackingState;
	
	/*** FUNCTIONS ***/
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SetIsWithinAttackRange(bool Value);

	UFUNCTION()
	void Attack();
	
	//UFUNCTION()
	//void ProcessPerceivedInformation(AActor* Actor, FAIStimulus Stimulus);

	bool IsDead() const;
	
	/** Set AttackingState */
	//UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAttackingState(EAttackingState State) { AttackingState = State; }

	

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

	class ABaseAICharacter* OwnerCharacter= nullptr; 

	UFUNCTION(BlueprintCallable)
	void SetAIStateToAttacking();

};
