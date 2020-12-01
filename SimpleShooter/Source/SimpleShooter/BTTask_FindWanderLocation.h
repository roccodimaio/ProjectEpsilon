// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindWanderLocation.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_FindWanderLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UBTTask_FindWanderLocation();

	class ABaseAICharacter* OwnerPawn = nullptr;

	/*** VARIABLES ***/
	UPROPERTY(EditAnywhere)
	float SearchRadius = 200.f;

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	/*** VARIABLES ***/
	FVector OriginVector;
	FVector RandomLocationVector;

};
