// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	setupInputComponent();
	checkPhysicsHandleComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent)  // if a component is currently grabbed
	{
		physicsHandle->SetTargetLocation(getReachLineEnd());
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
	if (physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing its UPhysicsHandleComponent."), *GetOwner()->GetName());
	}
}

void UGrabber::grab()
{
	/// Line trace and see if we reach any actors with physics body collision channel set
	auto hitResult = getFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();  // gets the mesh in our case
	auto actorHit = hitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (actorHit)
	{
		physicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,  // no bones needed
			componentToGrab->GetOwner()->GetActorLocation(),
			true  // allow rotation
		);
	}
}

void UGrabber::grabRelease()
{
	physicsHandle->ReleaseComponent();
}

FHitResult UGrabber::getFirstPhysicsBodyInReach() const
{
	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hitResult,
		getReachLineStart(),
		getReachLineEnd(),
		ECC_PhysicsBody
	);
	return hitResult;
}

FVector UGrabber::getReachLineStart() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);
	return playerViewPointLocation;
}

FVector UGrabber::getReachLineEnd() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		playerViewPointLocation,
		playerViewPointRotation
	);
	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}