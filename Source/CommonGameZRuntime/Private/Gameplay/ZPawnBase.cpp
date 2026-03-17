// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZPawnBase.h"

#include "EnhancedInputComponent.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Gameplay/ZActorExtensionChannels.h"
#include "Gameplay/Movement/ZActorMovementReplicationHandler.h"
#include "Scope/ZExtensionScope.h"
#include "Scope/ZPlayerExtensionScope.h"

AZPawnBase::AZPawnBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
	, bPendingDestroy(false)
{
}

void AZPawnBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);

	ZSharpReplicatedObject_GetLifetimeReplicatedProps(outLifetimeProps);
}

void AZPawnBase::PostNetReceiveLocationAndRotation()
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

void AZPawnBase::PostNetReceiveVelocity(const FVector& newVelocity)
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

void AZPawnBase::PostNetReceivePhysicState()
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

void AZPawnBase::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	if (auto component = CastChecked<UEnhancedInputComponent>(playerInputComponent))
	{
		ScriptSetupInputComponent(component);
	}
}

void AZPawnBase::BeginPlay()
{
	Super::BeginPlay();

	IZActorMovementReplicationHandler::FindHandler(this, MovementReplicationHandler);
}

void AZPawnBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}

void AZPawnBase::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);

	if (auto pc = Cast<APlayerController>(newController))
	{
		if (ULocalPlayer* player = pc->GetLocalPlayer())
		{
			if (auto playerScope = player->GetSubsystem<UZPlayerExtensionScope>())
			{
				playerScope->ExtensionScope_RegisterExtendee(this, CommonGameZ::TAG_ExtensionChannel_PawnPossessed);
			}
		}
	}
	
	if (auto controllerScope = Cast<IZExtensionScope>(newController))
	{
		controllerScope->ExtensionScope_RegisterExtendee(this, CommonGameZ::TAG_ExtensionChannel_PawnPossessed);
	}
}

void AZPawnBase::UnPossessed()
{
	if (!bPendingDestroy)
	{
		// IMPORTANT: This must be done before we call super because we need pointer to old controller.
		UnregisterExtendeeFromController(false);
	}

	Super::UnPossessed();
}

void AZPawnBase::DetachFromControllerPendingDestroy()
{
	if (!bPendingDestroy)
	{
		bPendingDestroy = true;

		// IMPORTANT: This must be done before we call super because we need pointer to old controller.
		UnregisterExtendeeFromController(true);
	}
	
	Super::DetachFromControllerPendingDestroy();
}

void AZPawnBase::UnregisterExtendeeFromController(bool destroying)
{
	if (auto controllerScope = Cast<IZExtensionScope>(Controller))
	{
		controllerScope->ExtensionScope_UnregisterExtendee(this, destroying);
	}
	
	if (auto pc = Cast<APlayerController>(Controller))
	{
		if (ULocalPlayer* player = pc->GetLocalPlayer())
		{
			if (auto playerScope = player->GetSubsystem<UZPlayerExtensionScope>())
			{
				playerScope->ExtensionScope_UnregisterExtendee(this, destroying);
			}
		}
	}
}


