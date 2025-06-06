// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	//Sets the name of the Node
	NodeName = "Find Random Location In Nav Mesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//If the owner of this Task has an AI Controller
	if(auto* const cont = Cast<ANPC_AIController>(OwnerComp.GetAIOwner()))
	{
		//and if it is a pawn
		if(auto* const npc = cont->GetPawn())
		{
			auto const Origin = npc->GetActorLocation();

			//Cast it to a nnew NPC
			auto npc_new = Cast<ANPC>(npc);

			//If this NPC should move
			if(npc_new->ShouldMove_)
			{
				//Get the naviagtable world
				if(auto* const NavSys= UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					FNavLocation Loc;
					//Gets a random point in that navigatable world
					if(NavSys->GetRandomPointInNavigableRadius(Origin,SearcRadius,Loc))
					{
						//Sets the owner blackboard components desired location to the new random location
						OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),Loc.Location);
					}
					//finish with success
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
			
		}
	}
	//If anything fails in terms of casting or needed components finish with failure
	return EBTNodeResult::Failed;
}
