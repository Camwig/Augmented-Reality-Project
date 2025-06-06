// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPenActor.h"

// Sets default values
AMyPenActor::AMyPenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Sets the scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
	//Sets the mesh to be attached to the scene component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	//Sets the materials
	auto redMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Red_Mat.Red_Mat'"));
	redMat = redMaterial.Object;

	auto defaultMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Default_Mat.Default_Mat'"));
	defaultMat = defaultMaterial.Object;

}

// Called when the game starts or when spawned
void AMyPenActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPenActor::SetMaterialColour(bool RedIsColour)
{
	if(RedIsColour)
	{
		//Sets the material based of if the pen should be set with a red or green colour
		ISRed=true;
		this->StaticMeshComponent->SetMaterial(0,redMat);
	}
	else
	{
		
		ISRed=false;
		this->StaticMeshComponent->SetMaterial(0,defaultMat);
	}
}

