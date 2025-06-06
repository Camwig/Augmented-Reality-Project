// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CustomGameMode.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "HelloARManager.h"
#include "MyPenActor.h"
#include "NPC.h"
#include "CustomARPawn.generated.h"

///

class UCameraComponent;

UCLASS()
class UE5_AR_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

	//NPC pointer to hold a tapped npc
	ANPC* npc_pointer_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	//void LineTrace(FVector);

	//Pointer to HelloManager
	AHelloARManager* HelloARManager;

	//Boolean to determine if the game mode should draw the planes
	bool should_draw;
	//Boolean to determine if the Pawn is holding an NPC
	bool isHoldingNPC;
	//Boolean to determine if the npc being held is red or not (Determines what kind of npc is currently being held
	bool isNPCRed;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Performs a hit test to check for a ray to find an actor
	bool WorldHitTest(FVector2D,FHitResult&);

	//Scene Component for pawn
	UPROPERTY(Category = "Scene pawn Component", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	//Camera Component
	UPROPERTY(Category = "Camera pawn component", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	//The Mesh used for the NPC
	UPROPERTY(Category = "Mesh pawn Component", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

	//NPC Class
	UPROPERTY(Category = "NPC pawn",VisibleAnywhere,BlueprintReadWrite)
		ANPC* npc;

	//The Materials used for the npcs
	UPROPERTY(Category = "Materials pawn", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* defaultMat;

	UPROPERTY(Category = "Materials pawn", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* redMat;

	//The Pen class component
	UPROPERTY(Category = "Pen pawn", VisibleAnywhere, BlueprintReadWrite)
		AMyPenActor* this_pen;
	
private:
	//Function to reset the pawn
	UFUNCTION()
	void ResetPawn();

	//Pointer to the game mode
	AGameModeBase* game_mode;
		
};
