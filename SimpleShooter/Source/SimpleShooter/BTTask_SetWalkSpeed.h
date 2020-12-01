// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetWalkSpeed.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UBTTask_SetWalkSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_SetWalkSpeed();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	/*** VARIABLES***/
	UPROPERTY(EditAnywhere)
	float NewMaxWalkSpeed = 300.f;
	
};
