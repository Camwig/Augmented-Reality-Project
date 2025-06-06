// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "HelloARManager.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

ACustomGameMode::ACustomGameMode()
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACustomARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();

	//Set the current stage to start as Start by default when starting the game
	Current_stage = Stage1::Start1;
}

void ACustomGameMode::StartPlay() 
{
	Super::StartPlay();
	//Calls the function to spawn and set the inital actors
	SpawnInitialActors();
	//Calls the Init function
	Init();
}

void ACustomGameMode::SpawnInitialActors()
{
	//Creates and sets the pointers for HelloARManager,CountdownActor and ScoreActor
	HelloManager = GetWorld()->SpawnActor<AHelloARManager>();
	count = GetWorld()->SpawnActor<ACountDownActor>(Countdown);
	new_score_ = GetWorld()->SpawnActor<AScoreActor>(Score_); 
}

void ACustomGameMode::CallCount()
{
	//If the game mode is to call the timer counter call its function
	if(count_time)
	{
		if(count!=nullptr)
			count->Count();
	}
}

// An implementation of the StartPlayEvent which can be triggered by calling StartPlayEvent() 
void ACustomGameMode::StartPlayEvent_Implementation() 
{

}

void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Switches based on the current stage
	switch(Current_stage)
	{
	case Stage1::Start1:
		//If it has not already been done and reset
		if(SetBroadcast == false)
		{
			//Set the broadcast to be true and the plane visibilty and the creation of new planes to also be false
			SetBroadcast=true;
		}
		break;

		
	case Stage1::Spawner_placement1:
		//If it has not already been done and reset
		if(SetBroadcast == false)
		{
			//Reset the mode in case of a repeated playthrough
			ResetMode();
			SetBroadcast=true;
		}

		//If the spawners have been placed
		if(Spawner_Vector.Num() >= 2)
		{
			//Change the current stage and reset the Broadcast variable
			Current_stage = Stage1::Bins_placement1;
			SetBroadcast=false;
		}
		break;
		
	case Stage1::Bins_placement1:

		//Reset the broadcast variable
		if(SetBroadcast == false)
		{
			SetBroadcast=true;
		}

		//If the pens have been placed
		if(Pen_Vector.Num() >= 2)
		{
			//Change the current stage and reset the Broadcast variable
			Current_stage = Stage1::Game1;
			SetBroadcast=false;
		}
		
		break;
		
	case Stage1::Game1:

		//If it has not already been done and reset
		if(SetBroadcast == false)
		{
			//Call the Innit Game stage
			Init();
			SetBroadcast=true;
		}

		//Set spawn red to a random number between 1 and 6
		SpawnRed = FMath::RandRange(1,6);
		//If there are items in the NPC array
		if(!NPC_Vector.IsEmpty())
		{
			//Check each item in the array
			for(int i = 0; i < NPC_Vector.Num();i++)
			{
				//If the item is to be destroyed
				if(NPC_Vector[i]->ShouldbeDeleted)
				{
					//Destroy that actor and remove it from the array
					NPC_Vector[i]->K2_DestroyActor();
					NPC_Vector.RemoveAt(i);
				}
			}
		}

		//If there has been a change in the number of items in the npc array
		if(ChangeINVector != NPC_Vector.Num())
		{
			//If the npc array is at the maximum of npcs allowed to exist at once
			if(NPC_Vector.Num() >= 6)
			{
				//spawners should not spawn
				ShoudlNPC_Spawn.Broadcast(0,0);
			}
			else
			{
				//Send the spawners the Spawnred value
				ShoudlNPC_Spawn.Broadcast(1,SpawnRed);
			}
		}

		//Set the ChangeINVector value to be that of the current size of the npc array
		ChangeINVector = NPC_Vector.Num();
		
		if(count!=nullptr)
		{
			//If the countdown actors TimeUp value is true
			if(count->TimeUp)
			{
				//Chnage the current stage
				Current_stage = Stage1::Loss1;
				//Tell the game mode to stop the timer
				count_time=false;
				//Broadcast the delegate for the pawn to reset
				To_Reset_pawn.Broadcast();
				//reset the Broadcast variable
				SetBroadcast=false;
			}
		}
		
		break;

	case Stage1::Loss1:

		if(SetBroadcast == false)
		{
			//Call the Init function
			Init();
			SetBroadcast=true;
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("YOU LOSE!"));
		
		break;
	}
	
}


void ACustomGameMode::LineTraceSpawnActor(FVector ScreenPos)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Line Trace Reached"));

	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);

	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			if (ActorPin)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ARPin is valid"));
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Trace Succeeded"));
    		
				const FActorSpawnParameters SpawnInfo;
				const FRotator MyRot(0, 0, 0);
	
				//Performs diffrent actions based on the current stage
				switch(Current_stage)
				{
				case Stage1::Spawner_placement1:
					//While the spawner array is less than the max of two
					if(Spawner_Vector.Num() < 2)
					{
						//Set the position of this new spawner and create it in the world
						const FVector MyLoc(TrackedTF.GetLocation().X, TrackedTF.GetLocation().Y, TrackedTF.GetLocation().Z+50.0f);
						ASpawnerActor* new_spawner = GetWorld()->SpawnActor<ASpawnerActor>(spawner,MyLoc,MyRot,SpawnInfo);

						//Set the colour of the spawner based in the order it is in the spawner array
						if(Spawner_Vector.Num() < 1)
						{
							new_spawner->SetMaterialColour(true);
						}
						else if(Spawner_Vector.Num() == 1)
						{
							new_spawner->SetMaterialColour(false);
						}
						//Add this new spawner to the array
						Spawner_Vector.Add(new_spawner);
					}
					break;
				case Stage1::Bins_placement1:
					//Performs the same set of actions as the spawner placement
					if(Pen_Vector.Num() < 2)
					{
						const FVector MyLoc(TrackedTF.GetLocation().X, TrackedTF.GetLocation().Y, TrackedTF.GetLocation().Z);
						AMyPenActor* new_pen = GetWorld()->SpawnActor<AMyPenActor>(Pen,MyLoc,MyRot,SpawnInfo);

						if(Pen_Vector.Num() < 1)
						{
							new_pen->SetMaterialColour(true);
						}
						else if(Pen_Vector.Num() == 1)
						{
							new_pen->SetMaterialColour(false);
						}
						
						Pen_Vector.Add(new_pen);
					}
					break;
				case Stage1::Game1:
					break;
				case Stage1::Loss1:
					break;
				}
			
			}
		}
	}
}

void ACustomGameMode::Add_NPC_to_array(ANPC* new_npc)
{
	//Adds the given npc to the npc vector
	NPC_Vector.Add(new_npc);
}

void ACustomGameMode::Init()
{
	
	switch(Current_stage)
	{
	case Stage1::Spawner_placement1:
		if(SetBroadcast == false)
		{
			///Broadcast for the spawners to spawn or not currently thats no
			ShoudlNPC_Spawn.Broadcast(0,0);
			SetBroadcast=true;
		}
		break;
	case Stage1::Bins_placement1:
		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(0,0);
			SetBroadcast=true;
		}
		break;
	case Stage1::Game1:

		if(SetBroadcast == false)
		{
			//Tell the spawners to start spawning
			ShoudlNPC_Spawn.Broadcast(1,0);
			SetBroadcast=true;
			//Start the timer
			count_time=true;
		}

		//Call the count down function every second
		GetWorldTimerManager().SetTimer(Ticker, this, &ACustomGameMode::CallCount, 1.0f, true, 0.0f);

		//If the spawner is not empty
		if(!Spawner_Vector.IsEmpty())
		{
			//loop through each spawner
			for(int i = 0; i < Spawner_Vector.Num();i++)
			{
				//Check the broadcast of the spawners and if they have a new npc to add
				ASpawnerActor* tempSpawner = Cast<ASpawnerActor>(Spawner_Vector[i]);
				tempSpawner->OnNPCToAdd.AddDynamic(this,&ACustomGameMode::Add_NPC_to_array);
			}
		}
		break;

	case Stage1::Loss1:
		
		if(SetBroadcast == false)
		{
			//Tell the spawners to stop spawning
			ShoudlNPC_Spawn.Broadcast(0,0);
			SetBroadcast=true;
		}

		TArray<AActor*> this_array;

		//If each array is not empty loop through each deleting and removing each element and removing them from the world in case
		if(!NPC_Vector.IsEmpty())
		{
			for(auto i : NPC_Vector)
			{
				i->K2_DestroyActor();
				NPC_Vector.Remove(i);
			}
			
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),NPC,this_array);

			for(auto j:this_array)
			{
				j->K2_DestroyActor();
				this_array.Remove(j);
			}
		}

		if(!Spawner_Vector.IsEmpty())
		{
			for(auto i : Spawner_Vector)
			{
				i->K2_DestroyActor();
				Spawner_Vector.Remove(i);
			}

			UGameplayStatics::GetAllActorsOfClass(GetWorld(),spawner,this_array);

			for(auto j:this_array)
			{
				j->K2_DestroyActor();
				this_array.Remove(j);
			}
		}

		if(!Pen_Vector.IsEmpty())
		{
			for(auto i : Pen_Vector)
			{
				i->K2_DestroyActor();
				Pen_Vector.Remove(i);
			}

			UGameplayStatics::GetAllActorsOfClass(GetWorld(),Pen,this_array);

			for(auto j:this_array)
			{
				j->K2_DestroyActor();
				this_array.Remove(j);
			}
		}
		
		break;
	}
}


int ACustomGameMode::ReturnMins()
{
	if(count!=nullptr)
	{
		return count->Mins;
	}
	return -2;
}

int ACustomGameMode::ReturnSecs()
{
	if(count!=nullptr)
	{
		return count->Secs;
	}
	return -2;
}

int ACustomGameMode::ReturnScore()
{
	if(new_score_!=nullptr)
	{
		return new_score_->score;
	}
	return -2;
}

void ACustomGameMode::CallAddTime()
{
	//Adds time to the timer and points to the score
	if(count!=nullptr)
		count->AddTime();
	if(new_score_!=nullptr)
		new_score_->AddPoints();
}

Stage1 ACustomGameMode::ReturnStage()
{
	return Current_stage;
}

void ACustomGameMode::SetStage(const Stage1 new_stage)
{
	Current_stage = new_stage;
}

void ACustomGameMode::ResetBraodcast()
{
	SetBroadcast = false;
}

void ACustomGameMode::ResetMode()
{
	//Reset each of these values to their inital value
	SetBroadcast=false;
	count_time=false;
	SpawnRed=0;
	ChangeINVector=0;
	if(new_score_!=nullptr)
	{
		//Reset the score actor
		new_score_->ResetScore();
	}
	if(count!=nullptr)
	{
		//Reset the count actor
		count->ResetCount();
	}
}


