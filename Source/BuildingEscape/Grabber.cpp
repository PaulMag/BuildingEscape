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

	// ...
	
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
}

