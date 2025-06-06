// Fill out your copyright notice in the Description page of Project Settings.
#include "NPC.h"

#include "ARPin.h"
#include "ARPlaneActor.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShouldMove_ = true;
	ShouldbeDeleted = false;
	Gravity = true;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		// Making sure the actor remains on the ARPin that has been found.
    	if(PinComponent)
    	{
    		auto TrackingState = PinComponent->GetTrackingState();
    		
    		switch (TrackingState)
    		{
    		case EARTrackingState::Tracking:
    			//Makes the component visible and sets it to a smaller scale
    			SceneComponent->SetVisibility(true);
    			SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
    			break;
    
    		case EARTrackingState::NotTracking:
    			PinComponent = nullptr;
    			break;
    
    		}
    	}
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* ANPC::GetBehaivourTree() const
{
	return Tree_;
}

void ANPC::OverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{
	//If gravity is still being applied to the npc
	if(Gravity)
	{
		//And if the npc contacts an AR Plane
		if(OtherActor->GetClass()->IsChildOf(AARPlaneActor::StaticClass()))
		{
			//Reduces the gravity scale to zero and sets the npcs gravity boolean to false
			this->GetCharacterMovement()->GravityScale = 0.0f;
			Gravity = false;
		}
	}
}





