// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::openDoor()
{
	owner->SetActorRotation(FRotator(0, openAngle, 0));
}
void UOpenDoor::closeDoor()
{
	owner->SetActorRotation(FRotator(0, closeAngle, 0));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume
	if (pressurePlate->IsOverlappingActor(actorThatOpens))
	{
		openDoor();
		lastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() > lastDoorOpenTime + doorCloseDelay)
	{
		closeDoor();
	}

}

