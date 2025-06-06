// Fill out your copyright notice in the Description page of Project Settings.

#include "NPC_AIController.h"
#include "NPC.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//Checks if the controller has possesed and NPC
	if(ANPC* const npc = Cast<ANPC>(InPawn))
	{
		//Gets the behaivour tree from the npc
		if(UBehaviorTree* const tree = npc->GetBehaivourTree())
		{
			//Creates a new blackboard component to be used and runs the behaivour tree
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset,b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}



