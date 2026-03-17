// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZSceneComponentBase.h"

#include "Emit/IZSharpFieldRegistry.h"

UZSceneComponentBase::UZSceneComponentBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{
	bWantsInitializeComponent = FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveInitialize))->GetOuterUClass() != GetClass();
}

void UZSceneComponentBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);

	ZSharpReplicatedObject_GetLifetimeReplicatedProps(outLifetimeProps);
}

ENetRole UZSceneComponentBase::GetLocalRole() const
{
	const AActor* owner = GetOwner();
	return owner ? owner->GetLocalRole() : ROLE_None;
}

ENetRole UZSceneComponentBase::GetRemoteRole() const
{
	const AActor* owner = GetOwner();
	return owner ? owner->GetRemoteRole() : ROLE_None;
}

bool UZSceneComponentBase::HasAuthority() const
{
	const AActor* owner = GetOwner();
	return owner && owner->HasAuthority();
}

void UZSceneComponentBase::InitializeComponent()
{
	Super::InitializeComponent();
	
	ReceiveInitialize();
}

void UZSceneComponentBase::BeginPlay()
{
	if (bIsZSharpClass)
	{
		ReceiveBeginPlay();
	}
	
	Super::BeginPlay();
}

void UZSceneComponentBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	if (!HasAnyFlags(RF_BeginDestroyed) && !IsUnreachable() && bIsZSharpClass)
	{
		ReceiveEndPlay(endPlayReason);
	}
	
	Super::EndPlay(endPlayReason);
}

void UZSceneComponentBase::Activate(bool reset)
{
	// We want to call ReceiveActivate() between SetActiveFlag() and OnComponentActivated so we directly rewrite this function.
	if (reset || ShouldActivate())
	{
		SetComponentTickEnabled(true);
		SetActiveFlag(true);

		ReceiveActivate();

		OnComponentActivated.Broadcast(this, reset);
	}
}

void UZSceneComponentBase::Deactivate()
{
	// We want to call ReceiveDeactivate() between SetActiveFlag() and OnComponentDeactivated so we directly rewrite this function.
	if (ShouldActivate())
	{
		SetComponentTickEnabled(false);
		SetActiveFlag(false);

		ReceiveDeactivate();

		OnComponentDeactivated.Broadcast(this);
	}
}

void UZSceneComponentBase::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	if (bIsZSharpClass && bHasZSharpTick)
	{
		ReceiveTick(deltaTime);

		// No need to process latent actions for native class.
	}
	
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
}


