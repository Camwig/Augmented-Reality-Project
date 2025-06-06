// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreActor.h"

// Sets default values
AScoreActor::AScoreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScoreActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScoreActor::AddPoints()
{
	score++;
}

void AScoreActor::ResetScore()
{
	score=0;
}



