// Fill out your copyright notice in the Description page of Project Settings.
#include "DummyGameMode.h"
#include "DummyARPawn.h"
//#include "CustomARPawn.h"
/*#include "AIController.h"*/
#include "CustomGameState.h"

//This is a version of the AR game mode that was used for testing in engine on my PC so most of it is still uncommented
//but it is unused throughout the actual AR application so I saw no need to add an internal commentry to it as I have already
//commented the AR Pawn version fo use

ADummyGameMode::ADummyGameMode()/*:
	new_npc(nullptr),new_ai(nullptr)*/
{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ADummyARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();

	Current_stage = Stage::Start;
	/*SetBroadcast=false;
	SpawnRed=0;*/
	change_ui=false;
	//ShouldBroadcast =false;
	
}

void ADummyGameMode::StartPlay()
{
	Super::StartPlay();
	count = GetWorld()->SpawnActor<ACountDownActor>(Countdown);
	new_score_ = GetWorld()->SpawnActor<AScoreActor>(Score_); 
	Init();
}

void ADummyGameMode::StartPlayEvent_Implementation()
{
	//GetWorldTimerManager().SetTimer(Ticker, this, &ADummyGameMode::CallCount, 1.0f, true, 0.0f);
}

void ADummyGameMode::CallCount()
{
	//CountDown_.Broadcast();
	if(count_time)
	{
		count->Count();
	}
}


void ADummyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Current npcs held in vector: %d"), NPC_Vector.Num()));

	switch(Current_stage)
	{
	case::Stage::Start:
		//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, TEXT("Start"));
		/*if(SetBroadcast == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reset"));#1#
			ShoudlNPC_Spawn.Broadcast(0,0);
			//ResetMode();
			SetBroadcast=true;
		}*/
		break;
	case Stage::Spawner_placement:
		
		if(SetBroadcast == false)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Reset"));
			//ShoudlNPC_Spawn.Broadcast(0,0);
			ResetMode();
			SetBroadcast=true;
		}

		
		if(Spawner_Vector.Num() >= 2)
		{
			Current_stage = Stage::Bins_placement;
			SetBroadcast=false;
		}
		break;
		
	case Stage::Bins_placement:
		
		if(SetBroadcast == false)
		{
			//ShoudlNPC_Spawn.Broadcast(0,0);
			SetBroadcast=true;
		}

		if(Pen_Vector.Num() >= 2)
		{
			Current_stage = Stage::Game;
			SetBroadcast=false;
		}
		
		break;
		
	case Stage::Game:

		if(SetBroadcast == false)
		{
			//ShoudlNPC_Spawn.Broadcast(1,0);
			Init();
			SetBroadcast=true;
		}

		SpawnRed = FMath::RandRange(1,6);
		
		
		if(!NPC_Vector.IsEmpty())
		{
			for(int i = 0; i < NPC_Vector.Num();i++)
			{
				if(NPC_Vector[i]->ShouldbeDeleted)
				{
					NPC_Vector[i]->K2_DestroyActor();
					NPC_Vector.RemoveAt(i);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Removed Item"));
				}
			}
		}

		if(ChangeINVector != NPC_Vector.Num())
		{
			//This seems to be picked up by the one so I will need to set some defintions for it to be done properly
			if(NPC_Vector.Num() >= 8)
			{
				ShoudlNPC_Spawn.Broadcast(0,0);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Should no longer spawn"));
			}
			else
			{
				ShoudlNPC_Spawn.Broadcast(1,SpawnRed);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Should now spawn"));
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Current npcs held in vector: %d"), NPC_Vector.Num()));

		/*TArray<AActor*> spawners;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASpawnerActor::StaticClass(),spawners);
	
		for(int i =0; i < spawners.Num();i++)
		{
			ASpawnerActor* tempSpawner = Cast<ASpawnerActor>(spawners[i]);
			tempSpawner->send_string.AddDynamic(this,&ADummyGameMode::PrintString);
		}*/

		ChangeINVector = NPC_Vector.Num();
		
		
		if(count->TimeUp)
		{
			Current_stage = Stage::Loss;
			To_Reset_pawn.Broadcast();
			count_time=false;
			SetBroadcast=false;
		}
		
		break;

	case Stage::Loss:

		if(SetBroadcast == false)
		{
			//ShoudlNPC_Spawn.Broadcast(0,0);
			Init();
			SetBroadcast=true;
		}

		
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("YOU LOSE!"));
		
		break;
	}
}

void ADummyGameMode::LineTraceSpawnActor(FVector ScreenPos)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Line Trace Reached"));

    //Basic variables for functionality
    APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
    FVector WorldPos;
    FVector WorldDir;

    //Gets the screen touch in world space and the tracked objects from a line trace from the touch
    UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);

    // construct trace vector (from point clicked to 1000.0 units beyond in same direction)
    FVector traceEndVector = WorldDir * 1000.0;
    traceEndVector = WorldPos + traceEndVector;

    FHitResult hitResult;

    // perform line trace (Raycast)
    bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, WorldPos, traceEndVector, ECollisionChannel::ECC_WorldStatic);

	if (traceSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Trace Succeeded"));
    		
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		//const FVector Test(324324, 032432, 0);
		//const FVector MyLoc(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);
    		
		//auto new_npc = GetWorld()->SpawnActor<ANPC>(NPC,MyLoc, MyRot, SpawnInfo);
		//new_npc->GetMesh()->SetWorldLocation(MyLoc);
		//auto new_ai = GetWorld()->SpawnActor<AAIController>(AI_Con);
		//new_ai->Possess(new_npc);
    
		//ANPC* new_npc;
		//UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(),NPC,Tree_,MyLoc, MyRot,true);
    		
		/*for(TActorIterator<ANPC> It(GetWorld(),ANPC::StaticClass());It;++It)
		{
			ANPC* npc = *It;
			if(npc != NULL)
			{
				NPC_Vector().Add(npc);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Added to array"));
			}
		}*/
	
	
		switch(Current_stage)
		{
		case Stage::Spawner_placement:
			if(Spawner_Vector.Num() < 2)
			{
				const FVector MyLoc(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z+100.0f);
				ASpawnerActor* new_spawner = GetWorld()->SpawnActor<ASpawnerActor>(spawner,MyLoc,MyRot,SpawnInfo);
				
				if(Spawner_Vector.Num() < 1)
				{
					new_spawner->SetMaterialColour(true);
				}
				else if(Spawner_Vector.Num() == 1)
				{
					new_spawner->SetMaterialColour(false);
				}
				
				Spawner_Vector.Add(new_spawner);
			}
			break;
		case Stage::Bins_placement:
			if(Pen_Vector.Num() < 2)
			{
				const FVector MyLoc(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);
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
		case Stage::Game:
			//Will need to get rid of this properly
			/*const FVector MyLoc(hitResult.Location.X, hitResult.Location.Y, hitResult.Location.Z);
			ANPC* new_npc = Cast<ANPC>(UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(),NPC,Tree_,MyLoc, MyRot,true));
			NPC_Vector.Add(new_npc);*/
			break;
		case Stage::Loss:
				break;
		}
	}
}

void ADummyGameMode::PrintString(ANPC* new_npc)
{
	NPC_Vector.Add(new_npc);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("ADDED NPC"));
}

void ADummyGameMode::Init()
{
	/*if(!Spawner_Vector.IsEmpty())
	{
		for(int i = 0; i < Spawner_Vector.Num();i++)
		{
			ASpawnerActor* tempSpawner = Cast<ASpawnerActor>(Spawner_Vector[i]);
			tempSpawner->OnNPCToAdd.AddDynamic(this,&ADummyGameMode::PrintString);
		}
	}*/

	//ShouldBroadcast =false;

	/*switch(Current_stage)
	{
	case Stage::Spawner_placement:
		break;
	case Stage::Bins_placement:
		break;
	case Stage::Game:
		break;
	}*/

	switch(Current_stage)
	{
	case Stage::Start:
		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(0,0);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit this part of INNIT"));
			SetBroadcast=true;
		}
		break;
	case Stage::Spawner_placement:
		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(0,0);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit this part of INNIT"));
			ResetMode();
			SetBroadcast=true;
		}
		break;
	case Stage::Bins_placement:
	    //Dont think there is a need for this 
		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(0,0);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit this part of INNIT"));
			SetBroadcast=true;
		}
		break;
	case Stage::Game:

		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(1,0);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit this part of INNIT"));
			SetBroadcast=true;
			count_time=true;
		}
		
		GetWorldTimerManager().SetTimer(Ticker, this, &ADummyGameMode::CallCount, 1.0f, true, 0.0f);
		
		//Be better to seperate the diffrent stages of the game into diffrent functions I think
		if(!Spawner_Vector.IsEmpty())
		{
			for(int i = 0; i < Spawner_Vector.Num();i++)
			{
				ASpawnerActor* tempSpawner = Cast<ASpawnerActor>(Spawner_Vector[i]);
				tempSpawner->OnNPCToAdd.AddDynamic(this,&ADummyGameMode::PrintString);
			}
		}
		break;

	case Stage::Loss:
		
		if(SetBroadcast == false)
		{
			ShoudlNPC_Spawn.Broadcast(0,0);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Hit this part of INNIT"));
			SetBroadcast=true;
		}

		TArray<AActor*> this_array;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Should lose 2"));
		if(!NPC_Vector.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Should lose 3"));
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
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Should lose 4"));
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

			change_ui=true;
		}

		if(Pen_Vector.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Emptied pen"));
		}

		if(Spawner_Vector.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Emptied spawner"));
		}

		if(NPC_Vector.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Emptied NPC"));
		}
		
		break;
		
	}

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Current npcs held in vector: %d"), NPC_Vector.Num()));
	//new_ai = GetWorld()->SpawnActor<AAIController>(AI_Con);

	//new_spawner->send_string.AddUniqueDynamic(this,&ADummyGameMode::PrintString);
}

int ADummyGameMode::ReturnMins()
{
	return count->Mins;
}

int ADummyGameMode::ReturnSecs()
{
	return count->Secs;
}

int ADummyGameMode::ReturnScore()
{
	return new_score_->score;
}

bool ADummyGameMode::ReturnDone()
{
	return change_ui;
}

Stage ADummyGameMode::ReturnStage()
{
	return Current_stage;
}

void ADummyGameMode::SetStage(const Stage new_stage)
{
	Current_stage = new_stage;
}

void ADummyGameMode::ResetBraodcast()
{
	SetBroadcast = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Reset 2"));
}


void ADummyGameMode::CallAddTime()
{
	//timer_add.Broadcast();
	count->AddTime();
	new_score_->AddPoints();
}

void ADummyGameMode::ResetMode()
{
	SetBroadcast=false;
	count_time=false;
	SpawnRed=0;
	ChangeINVector=0;
	new_score_->ResetScore();
	if(count!=nullptr)
	{
		count->ResetCount();
	}
}







