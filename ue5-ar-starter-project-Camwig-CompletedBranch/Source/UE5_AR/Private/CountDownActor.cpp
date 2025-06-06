// Fill out your copyright notice in the Description page of Project Settings.

#include "CountDownActor.h"

// Sets default values
ACountDownActor::ACountDownActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACountDownActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACountDownActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACountDownActor::Count()
{
	//If the timers seconds is not equal to zero
	//Decrease the seconds
	if(Secs!=0)
		Secs -=1;
	else
	{
		//If the minutes are equal to zero aswell as the seconds
		if(Mins==0)
		{
			//Set the Time up boolean to true
			TimeUp=true;
		}
		else
		{
			//If the minutes are not equal to zero decrease the minutes and set the seconds to 59
			Mins -=1;
			Secs=59;
		}
	}
}

void ACountDownActor::AddTime()
{
	//Add two seconds to the timer
	Secs += 2.0f;
}

void ACountDownActor::ResetCount()
{
	//Reset the values of the Timer
	Mins = 0;
	Secs = 25;
	TimeUp=false;
}



