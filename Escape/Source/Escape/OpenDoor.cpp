// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT
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
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle = InitialYaw - OpenAngle;
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor, but no PresurePlate Set"), *GetOwner()->GetName());
	}

	FindAudioComponent();
}

void UOpenDoor::FindAudioComponent() {
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("%s missing audio component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TotalMassOfActor() > MassToOpenDoor) {
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
		
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, OpenAngle, DeltaTime, DoorOpenSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	CloseDoorSound = false;
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("no audio component for open door"));
		return;}
	if (!OpenDoorSound) {
		UE_LOG(LogTemp, Error, TEXT("will play open sound"));
		AudioComponent->Play();
		OpenDoorSound = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	OpenDoorSound = false;
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("no audio component for close door"));
		return;}
	if (!CloseDoorSound) {
		UE_LOG(LogTemp, Error, TEXT("will play close sound"));
		AudioComponent->Play();
		CloseDoorSound = true;
	}

}

float UOpenDoor::TotalMassOfActor() const{
	float TotalMass = 0;

	TArray<AActor*> OverlappingActors;
	if(!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	for(AActor* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

