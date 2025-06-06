// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "HelloARManager.h"
#include "NPC.h"
#include "SpawnerActor.h"
#include "MyPenActor.h"
#include "ScoreActor.h"
#include "CountDownActor.h"
#include "Containers/Array.h"
#include "CustomGameMode.generated.h"

//Delegate to be used to tell the spawners if they should spawn
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShouldSpawn1,int32,now_spawn,int32,Should_spawn);

//Delegate to tell the pawn to reset itself
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetPawn1);

//Enum to determine the stage of the game and what it should be doing
UENUM(BlueprintType)
enum class Stage1 : uint8
{
	Spawner_placement1 = 0 UMETA(DisplayName="Spawner_Placement"),
	Bins_placement1 = 1 UMETA(DisplayName="Bins_placement"),
	Game1 = 2 UMETA(DisplayName="Game"),
	Loss1 =3 UMETA(DisplayName="Loss"),
	Start1 =4 UMETA(DisplayName = "Start")
};

UCLASS()
class UE5_AR_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:
	//Time handler used to call a function in a set time frame
	FTimerHandle Ticker;
	//Function to add a spawned npc to a vector
	UFUNCTION()
	void Add_NPC_to_array(ANPC* new_npc);
	
public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;

	virtual void StartPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "GameModeBase", DisplayName = "Start Play")
		void StartPlayEvent();
	
	virtual void Tick(float DeltaSeconds) override;

	//Function to spawn an actor based of a line trace
	virtual void LineTraceSpawnActor(FVector ScreenPos);

	//Function to spawn some initail actors when the game begins
	virtual void SpawnInitialActors();

	//Property used to hold the score actor refrence
	UPROPERTY(Category = "Score",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<AScoreActor> Score_;

	//Property hold a refence to the HelloARManager used to control and setup the AR session
	UPROPERTY(Category = "myCtaegory",VisibleAnywhere,BlueprintReadWrite)
		AHelloARManager* HelloManager;

	//Property used to hold a refrence to count down actor
	UPROPERTY(Category = "Timer",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ACountDownActor> Countdown;

	//Property used to hold a refence to an NPC actor
	UPROPERTY(Category = "NPC",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ANPC> NPC;

	//Property used to hold a refence to a spawner actor
	UPROPERTY(Category = "Spawner",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ASpawnerActor> spawner;

	//Property used to hold a refence to a Pen actor
	UPROPERTY(Category = "Pen Actor",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<AMyPenActor> Pen;

	//Used to create a property of the delegates to broadcast
	UPROPERTY(BlueprintAssignable)
		FShouldSpawn1 ShoudlNPC_Spawn;

	UPROPERTY(BlueprintAssignable)
		FResetPawn1 To_Reset_pawn;

	//Blueprint callable function to return and set certain variables
	UFUNCTION(BlueprintCallable,BlueprintPure)
	int ReturnMins();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int ReturnSecs();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int ReturnScore();

	UFUNCTION()
	void CallAddTime();

	UFUNCTION(BlueprintCallable)
	Stage1 ReturnStage();

	UFUNCTION(BlueprintCallable)
	void  SetStage(const Stage1 new_stage);

	UFUNCTION(BlueprintCallable)
	void  ResetBraodcast();
private:
	//Array of npc pointers to store the npcs currently in the scene
	TArray<ANPC*> NPC_Vector;
	//Array of npc pointers to store the spawners currently in the scene
	TArray<ASpawnerActor*> Spawner_Vector;
	//Array of npc pointers to store the pens currently in the scene
	TArray<AMyPenActor*> Pen_Vector;
	//Used to show a change in the npc vector size
	int ChangeINVector=0;
	//Refrence to the current stage the game is in
	Stage1 Current_stage;
	//Boolean to tell the intial setup of each stage if they need to broadcast and call intialisers
	bool SetBroadcast;
	//Init Function
	void Init();
	//Value that will be used to determine which spanwer of each colour will be allowed to spawn
	int SpawnRed;
	//Function to call the countdown actors count variable
	void CallCount();
	//Pointer to a countdown actor
	ACountDownActor* count;
	//Pointer to a score actor
	AScoreActor* new_score_;
	//Boolean to tell the game mode if it needs to run the timer or not
	bool count_time;
	//Function to reset the game mode
	void ResetMode();
};
