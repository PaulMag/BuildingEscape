// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	setupInputComponent();
	checkPhysicsHandleComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);
	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

void UGrabber::setupInputComponent()
{
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::grabRelease);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing its UInputComponent."), *GetOwner()->GetName());
	}
}

void UGrabber::checkPhysicsHandleComponent()
{
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle)
	{}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing its UPhysicsHandleComponent."), *GetOwner()->GetName());
	}
}

void UGrabber::grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."));
	auto hitResult = getFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();

	// If we hit something then attack a physics handle.
	// TODO: attach physics handle
	if (actorHit)
	{
		physicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true  // allow rotation
		);
	}
}

void UGrabber::grabRelease()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released."));
	physicsHandle->ReleaseComponent();
}

FHitResult UGrabber::getFirstPhysicsBodyInReach() const
{
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

	return hit;
}
