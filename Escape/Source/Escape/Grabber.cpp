// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Grabber.h"

#define OUT
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

	FindPhysicsHandle();

	SetUpInputComponent();
}

void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {

	} else {
		UE_LOG(LogTemp, Error, TEXT("%s does not have required Physics Handle Component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("found input Component"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("input Component NOT found"));
	}
}

void UGrabber::Grab() {


	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation(
		ComponentToGrab,
		NAME_None,
		GetPlayerReach()
		);
	}

}

void UGrabber::Release() {
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}


FVector UGrabber::GetPlayerReach() const {
		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation, 
			OUT PlayerViewPointRotation
		);
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	DrawDebugLine(
		GetWorld(),
		GetPlayerWorldPos(),
		GetPlayerReach(),
		FColor(0, 255, 0),
		false,
		0.f,
		0,
		5.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPos(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
		);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}

FVector UGrabber::GetPlayerWorldPos() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

