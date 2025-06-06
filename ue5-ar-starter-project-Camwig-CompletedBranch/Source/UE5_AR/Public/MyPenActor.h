// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPenActor.generated.h"

UCLASS()
class UE5_AR_API AMyPenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPenActor();
	//Boolean to keep track what kind of pen an instance is (red or green)
	bool ISRed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Function to set the material colour of the pen
	void SetMaterialColour(bool RedIsColour);
	//Property for the scene component
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;
	//Proprety for the mesh component
	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMeshComponent;
	//Property for the two materials for the mesh to be set to
	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* defaultMat;

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
		UMaterialInterface* redMat;

};
