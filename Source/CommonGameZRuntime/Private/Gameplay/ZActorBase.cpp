// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZActorBase.h"

#include "Emit/IZSharpFieldRegistry.h"
#include "Gameplay/Movement/ZActorMovementReplicationHandler.h"

AZActorBase::AZActorBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{
}

void AZActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);

	ZSharpReplicatedObject_GetLifetimeReplicatedProps(outLifetimeProps);
}

void AZActorBase::PostNetReceiveLocationAndRotation()
{
	if (!(MovementReplicationHandler && MovementReplicationHandler->ActorMovementReplicationHandler_CanHandleLocationAndRotation()))
	{
		Super::PostNetReceiveLocationAndRotation();
		return;
	}

	const FRepMovement& localRepMovement = GetReplicatedMovement();
	FVector newLocation = FRepMovement::RebaseOntoLocalOrigin(localRepMovement.Location, this);

	if (!MovementReplicationHandler->ActorMovementReplicationHandler_PostNetReceiveLocationAndRotation(newLocation, localRepMovement.Rotation))
	{
		Super::PostNetReceiveLocationAndRotation();
	}
}

void AZActorBase::PostNetReceiveVelocity(const FVector& newVelocity)
{
	if (!(MovementReplicationHandler && MovementReplicationHandler->ActorMovementReplicationHandler_CanHandleVelocity()))
	{
		Super::PostNetReceiveVelocity(newVelocity);
		return;
	}

	if (!MovementReplicationHandler->ActorMovementReplicationHandler_PostNetReceiveVelocity(newVelocity))
	{
		Super::PostNetReceiveVelocity(newVelocity);
	}
}

void AZActorBase::PostNetReceivePhysicState()
{
	if (!(MovementReplicationHandler && MovementReplicationHandler->ActorMovementReplicationHandler_CanHandlePhysicState()))
	{
		Super::PostNetReceivePhysicState();
		return;
	}

	const FRepMovement& localRepMovement = GetReplicatedMovement();
	FRigidBodyState rbstate;
	localRepMovement.CopyTo(rbstate, this);

	if (!MovementReplicationHandler->ActorMovementReplicationHandler_PostNetReceivePhysicState(rbstate))
	{
		Super::PostNetReceivePhysicState();
	}
}

void AZActorBase::BeginPlay()
{
	Super::BeginPlay();

	IZActorMovementReplicationHandler::FindHandler(this, MovementReplicationHandler);
}

void AZActorBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}


