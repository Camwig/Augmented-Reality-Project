// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
//#include "PlaceableActor.h"
#include "MyPenActor.h"
#include "NPC.h"

//This is a version of the AR pawn that was used for testing in engine on my PC so most of it is still uncommented
//but it is unused throughout the actual AR application so I saw no need to add an internal commentry to it as I have already
//commented the AR Pawn version fo use

#include "DummyARPawn.generated.h"

UCLASS()
class UE5_AR_API ADummyARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADummyARPawn();

	ANPC* npc_pointer_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Pawn")
	virtual void MoveStrafe(float Val);
	
	UFUNCTION(BlueprintCallable, Category="Pawn")
	virtual void MoveForwards(float Val);

	UFUNCTION(BlueprintCallable, Category="Pawn")
	virtual void MoveVertical(float Val);

	UFUNCTION(BlueprintCallable, Category="Pawn")
	virtual void LookYaw(float Rate);

	UFUNCTION(BlueprintCallable, Category="Pawn")
	virtual void LookPitch(float Rate);

	//virtual UPawnMovementComponent* GetMovementComponent() const override;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pawn")
	//float BaseTurnRate;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pawn")
	//float BaseLookUpRate;

	//UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UPawnMovementComponent> MovementComponent;

	bool isHoldingNPC;
	bool isNPCRed;

	float Speedforwards = 1000.0f;
	float SpeedStrafe = 1000.0f;
	float SpeedVertical =1000.0f;

	FVector2d CameraInput;

	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	//Performs a hit test to check for a ray to find an actor
	bool WorldHitTest(FVector2D,FHitResult&);

	//virtual void LineTraceSpawnActor(FVector ScreenPos);

	//AHelloARManager* HelloARManager;

	//Boolean to track whether the finger is being held down on the touch screen
	bool isFingerDown;

	UPROPERTY(Category = "myCtaegory",VisibleAnywhere,BlueprintReadWrite)
		ANPC* npc;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
    	USceneComponent* SceneComponent;
    
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
    	UCameraComponent* CameraComponent;

	/*//Uproperty for the selected actor pointer
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		APlaceableActor* SelectedActor;*/

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* defaultMat;

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* redMat;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		AMyPenActor* this_pen;

private:
	UFUNCTION()
	void ResetPawn();

	AGameModeBase* game_mode;

	//bool GM_beenSet;
};
