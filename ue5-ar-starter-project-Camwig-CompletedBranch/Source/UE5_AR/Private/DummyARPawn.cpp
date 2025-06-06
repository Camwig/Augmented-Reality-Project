// Fill out your copyright notice in the Description page of Project Settings.

#include "DummyARPawn.h"

#include "DummyGameMode.h"

//This is a version of the AR pawn that was used for testing in engine on my PC so most of it is still uncommented
//but it is unused throughout the actual AR application so I saw no need to add an internal commentry to it as I have already
//commented the AR Pawn version fo use

// Sets default values
ADummyARPawn::ADummyARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SetRootComponent(CameraComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(CameraComponent);

	auto redMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Red_Mat.Red_Mat'"));
	redMat = redMaterial.Object;

	auto defaultMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/Default_Mat.Default_Mat'"));
	defaultMat = defaultMaterial.Object;

	//StaticMeshComponent->SetVisibility(false);

	//npc = GetWorld()->SpawnActor<ANPC>();
	
	//CameraComponent->SetupAttachment(SceneComponent);

}

// Called when the game starts or when spawned
void ADummyARPawn::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->SetWorldRotation(CameraComponent->GetComponentRotation());
	StaticMeshComponent->SetRelativeLocation(FVector (-100.0f,0,0));
	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	npc_pointer_ = nullptr;
	isHoldingNPC = false;

	//GM_beenSet=false;

	auto Temp = GetWorld()->GetAuthGameMode();
	game_mode = Cast<ADummyGameMode>(Temp);

	
	//StaticMeshComponent->SetWorldLocation(newloc);
	//npc = GetWorld()->SpawnActor<ANPC>();
}

// Called every frame
void ADummyARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FVector NewLoc = FVector(CameraComponent->GetComponentLocation().X + 1000.0f,CameraComponent->GetComponentLocation().Y,CameraComponent->GetComponentLocation().Z);

	//StaticMeshComponent->SetWorldRotation(CameraComponent->GetComponentRotation());
	//StaticMeshComponent->SetWorldLocation(NewLoc);

	FRotator newRotation = GetActorRotation();

	newRotation.Yaw += CameraInput.X;
	newRotation.Pitch += CameraInput.Y;

	SetActorRotation(newRotation);

	//tempSpawner->OnNPCToAdd.AddDynamic(this,&ADummyGameMode::PrintString);
	Cast<ADummyGameMode>(game_mode)->To_Reset_pawn.AddDynamic(this,&ADummyARPawn::ADummyARPawn::ResetPawn);


}

// Called to bind functionality to input
void ADummyARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ADummyARPawn::MoveForwards);
	PlayerInputComponent->BindAxis("Strafe", this, &ADummyARPawn::MoveStrafe);
	PlayerInputComponent->BindAxis("Vertical", this, &ADummyARPawn::MoveVertical);
	//PlayerInputComponent->BindAxis("DefaultPawn_Turn", this, &ADummyARPawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Yaw", this, &ADummyARPawn::LookYaw);
	//PlayerInputComponent->BindAxis("TurnAt", this, &ADummyARPawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Pitch", this, &ADummyARPawn::LookPitch);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADummyARPawn::OnScreenTouch);
}

void ADummyARPawn::MoveForwards(float scale) 
{
	FVector movement = GetActorForwardVector();
	//UKismetSystemLibrary::PrintString(this, movement.ToString(), true, true, FLinearColor(0, 0.66, 1, 1), 2);

	movement *= scale * GetWorld()->GetDeltaSeconds() * Speedforwards;
	AddActorWorldOffset(movement);
}

void ADummyARPawn::MoveStrafe(float scale) 
{
	FVector movement = GetActorRightVector();
	movement *= scale * GetWorld()->GetDeltaSeconds() * SpeedStrafe;
	AddActorWorldOffset(movement);
}

void ADummyARPawn::MoveVertical(float scale) 
{
	FVector movement = GetActorUpVector();
	movement *= scale * GetWorld()->GetDeltaSeconds() * SpeedVertical;
	AddActorWorldOffset(movement);
}

void ADummyARPawn::LookYaw(float scale)
{
	CameraInput.X = scale;
}

void ADummyARPawn::LookPitch(float scale) 
{
	CameraInput.Y = scale;
}

void ADummyARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ADummyGameMode>(Temp);
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ScreenTouch Reached"));

	FHitResult ActorHitResult;

	//Performs the world hit test
	if (WorldHitTest(FVector2D(ScreenPos.X, ScreenPos.Y), ActorHitResult))
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, TEXT("HitTestSuccessful"));

		// Get object of actor hit.

		//Get the class actor that has been gotten from the hit result
		UClass* hitActorClass = UGameplayStatics::GetObjectClass(ActorHitResult.GetActor());

		//Checks if the hit object is an actor
		/*if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaceableActor::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Purple, TEXT("FoundActor"));
		}
		else */if(UKismetMathLibrary::ClassIsChildOf(hitActorClass,AMyPenActor::StaticClass()))
		{
			if(isHoldingNPC)
			{
				//Issue here
				this_pen = Cast<AMyPenActor>(ActorHitResult.GetActor());
				if(this_pen->ISRed && isNPCRed)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Found pen now deposit npc!"));
					StaticMeshComponent->SetRelativeLocation(FVector (-500.0f,0,0));
					StaticMeshComponent->SetVisibility(false);
					GM->CallAddTime();
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

				///delete this_pen;
				//this_pen = nullptr;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("no npc to deposit"));
			}
			//npc->ShouldMove_=false;
		}
		else if(UKismetMathLibrary::ClassIsChildOf(hitActorClass,ANPC::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("efrwtset"));
			if(isHoldingNPC)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Already holding npc"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Found NPC!"));
				npc_pointer_=Cast<ANPC>(ActorHitResult.GetActor());
				npc_pointer_->ShouldbeDeleted=true;

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
				
				npc_pointer_ = nullptr;
				StaticMeshComponent->SetRelativeLocation(FVector (250.0f,0,0));
				StaticMeshComponent->SetVisibility(true);
				
				isHoldingNPC =true;
			}
			
		}


		//if(UKismetMathLibrary::ClassIsChildOf())
	}
	/*
	else if (SelectedActor != NULL)
	{
		//Deselect the actor in the selected actor
		SelectedActor->StaticMeshComponent->SetMaterial(0, defaultMat);
		SelectedActor = NULL;

		//Call the game mode function to set the planes invisible
		//GM->HelloManager->MakePlanesVisible();
		GM->HelloManager->SetPlaneVisibility(true);
		GM->HelloManager->ShouldCreateNewPlanes(true);
	}
	*/

	else if (GM)
	{
		// Add call to the line-trace here from the Custom Game Mode
		GM->LineTraceSpawnActor(ScreenPos);
	}
	

	//Sets the finger to being held down
	isFingerDown = true;

}

bool ADummyARPawn::WorldHitTest(FVector2D screenPos, FHitResult& hitResult)
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

void ADummyARPawn::ResetPawn()
{
	StaticMeshComponent->SetVisibility(false);
	npc_pointer_ = nullptr;
	isHoldingNPC = false;
}


