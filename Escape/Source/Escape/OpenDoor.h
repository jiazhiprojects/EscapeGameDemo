// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActor() const;
	void FindAudioComponent();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;

private:
	float InitialYaw;
	float CurrentYaw;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 7.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
		
	UPROPERTY()
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;	

};
