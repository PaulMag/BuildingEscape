// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle)
	{}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing its UPhysicsHandleComponent."), *GetOwner()->GetName());
	}
	
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing its UInputComponent."), *GetOwner()->GetName());
	}
}

void UGrabber::grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);
	// Log out viewpoint
	//UE_LOG(LogTemp, Warning, TEXT("Player viewpoint at %s, %s\n"), *playerViewPointLocation.ToString(), *playerViewPointRotation.ToString());

	//FVector lineTraceEnd = playerViewPointLocation + FVector(0, 0, 35.);
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;
	DrawDebugLine(
		GetWorld(),
		playerViewPointLocation,  // from
		lineTraceEnd,             // to
		FColor(255, 0, 128),
		false,
		0,
		0,
		10
	);
	
	// Set query parameters
	//FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		playerViewPointLocation,
		lineTraceEnd,
		ECC_PhysicsBody
	);
		//FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		//traceParameters,

	// See what we hit
	AActor* hitActor = hit.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit actor: %s\n"), *(hitActor->GetName()));
	}	
	
	//UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s\n"), hit.GetActor()->GetName().ToString());
}

