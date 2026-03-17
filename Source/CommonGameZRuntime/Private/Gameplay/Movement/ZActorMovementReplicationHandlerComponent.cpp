// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/Movement/ZActorMovementReplicationHandlerComponent.h"

#include "Kismet/KismetMathLibrary.h"

UZActorMovementReplicationHandlerComponent::UZActorMovementReplicationHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

bool UZActorMovementReplicationHandlerComponent::ActorMovementReplicationHandler_PostNetReceiveLocationAndRotation(const FVector& newLocation, const FRotator& newRotation)
{
	const AActor* owner = GetOwner();
	if (!owner)
	{
		return false;
	}

	const FTransform& transform = owner->GetTransform();
	if (newLocation == transform.GetLocation() && newRotation == transform.GetRotation().Rotator())
	{
		return false;
	}

	TargetLocation = newLocation;
	TargetRotation = newRotation;
	bInterpolating = true;
	SetComponentTickEnabled(true);
	
	return true;
}

void UZActorMovementReplicationHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!bInterpolating)
	{
		SetComponentTickEnabled(false);
	}
}

void UZActorMovementReplicationHandlerComponent::TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	AActor* owner = GetOwner();
	if (!owner)
	{
		return;
	}
	
	if(owner->GetRootComponent() && owner->GetRootComponent()->IsRegistered())
	{
		FVector velocity = owner->GetReplicatedMovement().LinearVelocity;
		FVector location = UKismetMathLibrary::VLerp(owner->GetActorLocation(), TargetLocation, 10 * deltaTime) + velocity * deltaTime;
		FRotator rotation = UKismetMathLibrary::RLerp(owner->GetActorRotation(), TargetRotation, 20 * deltaTime, true);
		owner->SetActorLocationAndRotation(location, rotation);
	}
}


