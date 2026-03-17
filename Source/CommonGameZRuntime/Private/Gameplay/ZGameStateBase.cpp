// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZGameStateBase.h"

#include "ZActorExtensionHelper.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Gameplay/ZClientGameControllerBase.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

CommonGameZ::ZGameStateBase_Private::FZSetClientGameControllerClass::FZSetClientGameControllerClass(AZGameStateBase* gameState, TSubclassOf<AZClientGameControllerBase> controllerClass)
{
	gameState->SetClientGameControllerClass(controllerClass);
}

AZGameStateBase::AZGameStateBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{
}

void AZGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);

	FDoRepLifetimeParams params;
	params.bIsPushBased = true;
	{
		auto& OutLifetimeProps = outLifetimeProps;
		DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ClientGameControllerClass, params);
	}
	
	ZSharpReplicatedObject_GetLifetimeReplicatedProps(outLifetimeProps);
}

void AZGameStateBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZGameStateBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}

void AZGameStateBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);
	
	Super::EndPlay(endPlayReason);
}

void AZGameStateBase::OnRep_ClientGameControllerClass()
{
	ReceivedClientGameControllerClass();
}

void AZGameStateBase::SetClientGameControllerClass(TSubclassOf<AZClientGameControllerBase> controllerClass)
{
	ClientGameControllerClass = controllerClass;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, ClientGameControllerClass, this);
	
	ReceivedClientGameControllerClass();
}

void AZGameStateBase::ReceivedClientGameControllerClass()
{
	if (IsRunningDedicatedServer())
	{
		return;
	}

	if (!GetWorld()->IsGameWorld())
	{
		return;
	}

	verify(GetWorld()->SpawnActor(ClientGameControllerClass));
}


