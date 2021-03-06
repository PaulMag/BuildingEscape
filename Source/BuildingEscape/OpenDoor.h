// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void openDoor();
	void closeDoor();

private:
	AActor * owner;

	UPROPERTY(EditAnywhere)
	float openAngle = 20.0;
	UPROPERTY(EditAnywhere)
	float closeAngle = 90.0;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressurePlate;
	
	UPROPERTY(EditAnywhere)
	float doorCloseDelay = 0.1;
	float lastDoorOpenTime;

	//UPROPERTY(EditAnywhere)
	AActor* actorThatOpens;
};
