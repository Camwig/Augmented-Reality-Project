// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "NPC.generated.h"

class UARPin;

UCLASS()
class UE5_AR_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Behaivour tree pointer used to determine the npcs behaivour
	UBehaviorTree* GetBehaivourTree() const;
	//Property to determine if the npc should move
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess ="true"))
		bool ShouldMove_;
	//Property to determine if the npc should be deletd from the world
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess ="true"))
	bool ShouldbeDeleted;
	//Property for the scene component
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;
	//AR Pin	
	UARPin* PinComponent;
	//Boolean to determine what colour the npc is
	bool ThisIsRed;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Behaivour tree property to be used for the npc
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess ="true"))
		UBehaviorTree* Tree_;
	//Function to determine if the npc has hit a certain objecy in this case AR planes
	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	//Boolean to say whether the npc has gravity applied to it or not
	bool Gravity;
};
