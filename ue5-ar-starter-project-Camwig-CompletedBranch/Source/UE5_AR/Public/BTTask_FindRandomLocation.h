// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "NPC.h"

#include "BTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	//Creates a new Task for a Blueprint Behaivour tree by creating both a constructor and an override of execute task
	explicit UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//Creates a new property that is readable by blueprints for the radius the NPCs are allowed to search to find a new destination to move to
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess="true"))
		float SearcRadius = 200.0f;
};
