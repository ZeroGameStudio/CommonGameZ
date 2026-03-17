// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZActorMovementReplicationHandler.generated.h"

UINTERFACE(meta = (CannotImplementInterfaceInBlueprint))
class UZActorMovementReplicationHandler : public UInterface
{
	GENERATED_BODY()
};

class COMMONGAMEZRUNTIME_API IZActorMovementReplicationHandler : public IInterface
{
	GENERATED_BODY()

public:
	static bool FindHandler(AActor* self, TScriptInterface<IZActorMovementReplicationHandler>& handler);

public:
	virtual bool ActorMovementReplicationHandler_CanHandleLocationAndRotation() const { return false; }
	virtual bool ActorMovementReplicationHandler_CanHandleVelocity() const { return false; }
	virtual bool ActorMovementReplicationHandler_CanHandlePhysicState() const { return false; }
	
	virtual bool ActorMovementReplicationHandler_PostNetReceiveLocationAndRotation(const FVector& newLocation, const FRotator& newRotation) { PURE_VIRTUAL(ActorMovementReplicationHandler_PostNetReceiveLocationAndRotation); return false; }
	virtual bool ActorMovementReplicationHandler_PostNetReceiveVelocity(const FVector& newVelocity) { PURE_VIRTUAL(ActorMovementReplicationHandler_PostNetReceiveVelocity); return false; }
	virtual bool ActorMovementReplicationHandler_PostNetReceivePhysicState(FRigidBodyState& rbstate) { PURE_VIRTUAL(ActorMovementReplicationHandler_PostNetReceivePhysicState); return false; }
	
};


