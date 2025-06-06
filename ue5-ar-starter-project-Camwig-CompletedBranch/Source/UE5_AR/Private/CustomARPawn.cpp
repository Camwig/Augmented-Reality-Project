// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACustomARPawn::ACustomARPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	//Sets the camera component attached to the scene
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SceneComponent);

	//Setsup the mesh component attached to the camera component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(CameraComponent);

	//Sets the materails for both the default and red materials loaded from file
	auto redMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Red_Mat.Red_Mat'"));
	redMat = redMaterial.Object;

	auto defaultMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Default_Mat.Default_Mat'"));
	defaultMat = defaultMaterial.Object;

	//Sets the planes to be drawn to true
	should_draw=true;
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

	//Sets the mesh attached to the camera component to be invisible and no collidable
	//This will be used to create the illusion that an npc when tapped is being picked up
	StaticMeshComponent->SetWorldRotation(CameraComponent->GetComponentRotation());
	StaticMeshComponent->SetRelativeLocation(FVector (-500.0f,0,0));
	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Sets the npc pointer to be null as well as the boolean to dictate whether an npc is being held or not to false
	npc_pointer_ = nullptr;
	isHoldingNPC = false;

	//Sets the game mode pointer to tehe current game mode
	auto Temp = GetWorld()->GetAuthGameMode();
	game_mode = Cast<ACustomGameMode>(Temp);
}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Checks if a delegate has been broadcast to call the reset pawn function
	Cast<ACustomGameMode>(game_mode)->To_Reset_pawn.AddDynamic(this,&ACustomARPawn::ResetPawn);
	
}

bool ACustomARPawn::WorldHitTest(FVector2D screenPos, FHitResult& hitResult)
{
	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector

	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/ worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;

	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, worldPosition, traceEndVector, ECollisionChannel::ECC_WorldDynamic);

	// return if the operation was successful
	return traceSuccess;
}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Binds the touch event to the OnScreenTouched function
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	if(should_draw)
	{
		//On intial touch set the plane visibilty to true and to create new planes
		GM->HelloManager->SetPlaneVisibility(true);
		GM->HelloManager->ShouldCreateNewPlanes(true);
		should_draw=false;
	}

	FHitResult ActorHitResult;

	//Performs the world hit test
	if (WorldHitTest(FVector2D(ScreenPos.X, ScreenPos.Y), ActorHitResult))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, TEXT("HitTestSuccessful"));

		// Get object of actor hit.

		//Get the class actor that has been gotten from the hit result
		UClass* hitActorClass = UGameplayStatics::GetObjectClass(ActorHitResult.GetActor());

		//If the actor is of the type Pen
		if(UKismetMathLibrary::ClassIsChildOf(hitActorClass,AMyPenActor::StaticClass()))
		{
			//If the pawn is holding an NPC
			if(isHoldingNPC)
			{
				//Casts the hit actor as a Pen_Actor
				this_pen = Cast<AMyPenActor>(ActorHitResult.GetActor());
				//Checks if both the NPC and pen match colours
				if(this_pen->ISRed && isNPCRed)
				{
					//Reset the pawn mesh position and visibilty
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Found pen now deposit npc!"));
					StaticMeshComponent->SetRelativeLocation(FVector (-500.0f,0,0));
					StaticMeshComponent->SetVisibility(false);
					//Call the add time function
					GM->CallAddTime();
					//Set the Npc being held to false
					isHoldingNPC =false;
				} 
				else if(!this_pen->ISRed && !isNPCRed)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Found pen now deposit npc!"));
					StaticMeshComponent->SetRelativeLocation(FVector (-500.0f,0,0));
					StaticMeshComponent->SetVisibility(false);
					GM->CallAddTime();
					isHoldingNPC =false;
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("npc and pen do not match!"));
				}
				
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("no npc to deposit"));
			}
		}
		else if(UKismetMathLibrary::ClassIsChildOf(hitActorClass,ANPC::StaticClass()))
		{
			//Checks if the pawn is already holding an npc
			if(isHoldingNPC)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Already holding npc"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Found NPC!"));
				//Cast the actor as an npc
				npc_pointer_=Cast<ANPC>(ActorHitResult.GetActor());
				//Set the npc to be deleted
				npc_pointer_->ShouldbeDeleted=true;

				//Set the colour of the pawn mesh based off the colour of the npc
				if(npc_pointer_->ThisIsRed)
				{
					StaticMeshComponent->SetMaterial(0,redMat);
					isNPCRed =true;
				}
				else
				{
					StaticMeshComponent->SetMaterial(0,defaultMat);
					isNPCRed =false;
				}
				//Set the npc pointer to null
				npc_pointer_ = nullptr;
				//Position the pawn mesh in front of the camera and set it be visible
				StaticMeshComponent->SetRelativeLocation(FVector (250.0f,0,0));
				StaticMeshComponent->SetVisibility(true);
				//Set he holding npc boolean to true
				isHoldingNPC =true;
			}
		}
	}

	if (GM)
	{
		// Add call to the line-trace here from the Custom Game Mode
		GM->LineTraceSpawnActor(ScreenPos);
	}

}

void ACustomARPawn::ResetPawn()
{
	//Reset the pawn
	StaticMeshComponent->SetVisibility(false);
	npc_pointer_ = nullptr;
	isHoldingNPC = false;
}



