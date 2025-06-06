// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActor.h"

#include "CustomGameMode.h"
#include "DummyGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASpawnerActor::ASpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Sets the mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//Sets the materials
	auto redMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Red_Mat.Red_Mat'"));
	redMat = redMaterial.Object;

	auto defaultMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Default_Mat.Default_Mat'"));
	defaultMat = defaultMaterial.Object;
}


// Called when the game starts or when spawned
void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	//Determines a random offset for the time between spawns
	int offset = FMath::RandRange(-1,1);
	//Allows for interspuced spawnning that doesnt happen every set couple of seconds
	//Calls the npc spawn function based off the timer
	GetWorldTimerManager().SetTimer(Ticker, this, &ASpawnerActor::SpawnNPC, 4.0f + offset, true, 0.0f);
	//Determines the size, origin and radius of the mesh of the spawner
	UKismetSystemLibrary::GetComponentBounds(StaticMeshComponent,new_origin,new_vector,radius);
	//Sets the spawner to not initialy spawn
	ShouldSpawn_ = false;
	//Checks the game mode delegate and if it is being braodcast
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	GM->ShoudlNPC_Spawn.AddDynamic(this,&ASpawnerActor::SetShouldSpawn);
}

// Called every frame
void ASpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnerActor::SpawnNPC()
{
	//Checks if the spawner should be active
	if(ShouldSpawn_)
	{
		//Spawning information and settings
		const FActorSpawnParameters SpawnInfo;
		const FRotator MyRot(0, 0, 0);
		//Gets the bounds of the mesh
		FVector SpawnOrigin = StaticMeshComponent->Bounds.Origin;
		FVector SpawnLimit = StaticMeshComponent->Bounds.BoxExtent;
		//Sets the spawning origin of the npc to be lightly off the spawner
		SpawnOrigin = FVector(SpawnOrigin.X - 30.0f,SpawnOrigin.Y, SpawnOrigin.Z + (SpawnLimit.Z*2));
		//Sets the location of the spawn
		const FVector MyLoc = UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin,SpawnLimit);
		//spawn a new npc
		ANPC* new_npc = Cast<ANPC>(UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(),NPC,Tree_,SpawnOrigin, MyRot,true));
		//Checks the colour of the spawner and sets the npc accordinly
		if(IsRed==true)
		{
			new_npc->GetMesh()->SetSkeletalMesh(AlternateMeshAsset);
			new_npc->ThisIsRed=true;
		}
		else
		{
			new_npc->GetMesh()->SetSkeletalMesh(AlternateMeshAsset2);
			new_npc->ThisIsRed=false;
		}
		//Broadcasts on a delegate the new npc
		OnNPCToAdd.Broadcast(new_npc);
		ShouldSpawn_ =false;
	}
}

void ASpawnerActor::SetShouldSpawn(int32 NowSpawn,int32 ShouldSpawn)
{
	//If Now spawn is one it is considered true (delegates did not like to play with booleans)
	if(NowSpawn == 1)
	{
		//IF Spawnred is an even number
		if(ShouldSpawn % 2)
		{
			//And the spawner itself is red it will spawn
			if(IsRed)
				ShouldSpawn_ = true;
		}
		else
		{
			//If spawn red is an odd number and the spawner is not red it will spawn
			if(!IsRed)
				ShouldSpawn_ = true;
		}
	}
	else if(NowSpawn == 0)
	{
		//If Now spawn is zero it does not spawn anything
		ShouldSpawn_ = false;
	}
}

void ASpawnerActor::SetMaterialColour(bool RedIsColour)
{
	//Sets the material colour of the spawner
	if(RedIsColour)
	{
		IsRed = true;
		this->StaticMeshComponent->SetMaterial(0,redMat);
	}
	else
	{
		IsRed = false;
		this->StaticMeshComponent->SetMaterial(0,defaultMat);
	}
}






