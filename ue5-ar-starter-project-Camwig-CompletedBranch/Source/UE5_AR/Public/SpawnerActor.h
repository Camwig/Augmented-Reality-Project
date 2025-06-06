// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.h"
#include "SpawnerActor.generated.h"
//Delegate to tell the game mode to add the spawned npc to an array
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FADD_NPC,ANPC*,New_npc);

UCLASS()
class UE5_AR_API ASpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnerActor();
	//Boolean to determine if the spawner should be active
	bool ShouldSpawn_;
	//Bollean to determine the cpolour of the object
	bool IsRed;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//Function to spawn an npc
	void SpawnNPC();
	//Time handler to handle the calling of the spawner function
	FTimerHandle Ticker;
	//Variables to hold the defintions of the spawners mesh component
	FVector new_vector;
	FVector new_origin;
	float radius;
	//Static mesh component property
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Sets the material of the mesh
	void SetMaterialColour(bool RedIsColour);
	//Property of the npc to sawn
	UPROPERTY(Category = "NPC",EditAnywhere,BlueprintReadWrite)
		TSubclassOf<ANPC> NPC;
	//Tree to be spawned with the npc
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI",meta=(AllowPrivateAccess ="true"))
		UBehaviorTree* Tree_;
	//Delegate to be broadcast
	UPROPERTY(BlueprintAssignable)
		FADD_NPC OnNPCToAdd;
	//The two material properties
	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* defaultMat;

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
	UMaterialInterface* redMat;
	//Mesh asset to help show which colour the spawner is
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
		USkeletalMesh* AlternateMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
		USkeletalMesh* AlternateMeshAsset2;
private:
	//Delegated function told to run by the game mode
	UFUNCTION()
	void SetShouldSpawn(int32 NowSpawn,int32 ShouldSpawn);
};
