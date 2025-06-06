// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CountDownActor.generated.h"

UCLASS()
class UE5_AR_API ACountDownActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACountDownActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Boolean value to let the game mode know that time is up and to change state
	bool TimeUp = false;

	//Integer value used to represent the minutes on the imer (No longer used as I found for a better gameplay experience starting the timer at 25 seconds was better)
	UPROPERTY(BlueprintReadOnly)
	int Mins = 0;

	//Integer value to represent the seconds left on the timer
	UPROPERTY(BlueprintReadOnly)
	int Secs = 25;

	//Function to countdown
	UFUNCTION()
	void Count();

	//Function to increase the time on the timer when adding an NPC to the pen
	UFUNCTION()
	void AddTime();

	//Function to reset the timer
	void ResetCount();
};
