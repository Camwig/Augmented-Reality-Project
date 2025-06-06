// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ARBlueprintLibrary.h"
//#include "PlaceableActor.h"
#include "NPC.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "CountDownActor.h"
#include "ScoreActor.h"
#include "MyPenActor.h"
#include "Containers/Array.h"
#include "GameFramework/GameModeBase.h"

#include "SpawnerActor.h"

#include "DummyGameMode.generated.h"

//This is a version of the AR game mode that was used for testing in engine on my PC so most of it is still uncommented
//but it is unused throughout the actual AR application so I saw no need to add an internal commentry to it as I have already
//commented the AR Pawn version fo use

/**
 * 
 */

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSTRING_TO_SHOW,FString,string);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShouldSpawn,int32,now_spawn,int32,Should_spawn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCountDown);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAddToTimer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResetPawn);

UENUM(BlueprintType)
enum class Stage : uint8
{
	Spawner_placement = 0 UMETA(DisplayName="Spawner_Placement"),
	Bins_placement = 1 UMETA(DisplayName="Bins_placement"),
	Game = 2 UMETA(DisplayName="Game"),
	Loss=3 UMETA(DisplayName="Loss"),
	Start=4 UMETA(DisplayName = "Start")
};

class UCameraComponent;

UCLASS()
class UE5_AR_API ADummyGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADummyGameMode();
	virtual ~ADummyGameMode() = default;

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartPlay() override;

	virtual void LineTraceSpawnActor(FVector ScreenPos);
	
	/*UPROPERTY(Category = "UI",EditAnywhere,BlueprintReadWrite)
			UUserWidget* UI;*/

	UPROPERTY(Category = "Timer",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ACountDownActor> Countdown;

	UPROPERTY(Category = "Score",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<AScoreActor> Score_;

	UPROPERTY(Category = "NPC",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ANPC> NPC;
		
	//UPROPERTY(Category = "AI Controller",EditAnywhere,BlueprintReadWrite)
		//TSubclassOf<AAIController> AI_Con;

	UPROPERTY(Category = "Spawner",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ASpawnerActor> spawner;

	UPROPERTY(Category = "Pen Actor",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<AMyPenActor> Pen;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess ="true"))
		UBehaviorTree* Tree_;

	UPROPERTY(BlueprintAssignable)
		FShouldSpawn ShoudlNPC_Spawn;

	UPROPERTY(BlueprintAssignable)
		FCountDown CountDown_;

	UPROPERTY(BlueprintAssignable)
		FAddToTimer timer_add;

	UPROPERTY(BlueprintAssignable)
		FResetPawn To_Reset_pawn;

	UFUNCTION(BlueprintNativeEvent, Category = "GameModeBase", DisplayName = "Start Play")
		void StartPlayEvent();

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Array Stuff")
	TArray<ANPC*> NPC_Vector;

	TArray<ASpawnerActor*> Spawner_Vector;

	TArray<AMyPenActor*> Pen_Vector;

	UFUNCTION(BlueprintCallable,BlueprintPure)
		int ReturnMins();

	UFUNCTION(BlueprintCallable,BlueprintPure)
		int ReturnSecs();

	UFUNCTION(BlueprintCallable,BlueprintPure)
		int ReturnScore();

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool ReturnDone();

	UFUNCTION()
	void CallAddTime();

	UFUNCTION(BlueprintCallable)
	Stage ReturnStage();

	UFUNCTION(BlueprintCallable)
	void  SetStage(const Stage new_stage);

	UFUNCTION(BlueprintCallable)
	void  ResetBraodcast();
	
	//TArray<ASpawnerActor*> Spawner_Vector;
private:
	//ADummyARPawn* pawn;
	//APlaceableActor* SpawnedActor;
	//AAIController* new_ai;

	UFUNCTION()
	void PrintString(ANPC* new_npc);
	
	int ChangeINVector=0;

	Stage Current_stage;

	bool SetBroadcast;

	void Init();

	int SpawnRed;

	FTimerHandle Ticker;

	void CallCount();

	ACountDownActor* count;
	AScoreActor* new_score_;

	bool change_ui;

	bool count_time;

	void ResetMode();

	//TestClass* test;
};
