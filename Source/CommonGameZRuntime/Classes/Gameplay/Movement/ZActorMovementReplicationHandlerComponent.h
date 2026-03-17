// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Gameplay/ZActorComponentBase.h"
#include "ZActorMovementReplicationHandler.h"

#include "ZActorMovementReplicationHandlerComponent.generated.h"

UCLASS()
class COMMONGAMEZRUNTIME_API UZActorMovementReplicationHandlerComponent : public UZActorComponentBase, public IZActorMovementReplicationHandler
{
	GENERATED_BODY()

public:
	UZActorMovementReplicationHandlerComponent();

public:
	virtual bool ActorMovementReplicationHandler_CanHandleLocationAndRotation() const override { return true; }

	virtual bool ActorMovementReplicationHandler_PostNetReceiveLocationAndRotation(const FVector& newLocation, const FRotator& newRotation) override;

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
	bool bInterpolating;
	FVector TargetLocation;
	FRotator TargetRotation;
	
};


