// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZClientGameControllerBase.h"

#include "ZActorExtensionHelper.h"
#include "Gameplay/ZClientGameControllerSubsystemBase.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Resolver/ZExtenderCollection.h"
#include "Resolver/ZExtenderResolverBaseInterface.h"

AZClientGameControllerBase::AZClientGameControllerBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{
}

void AZClientGameControllerBase::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void AZClientGameControllerBase::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void AZClientGameControllerBase::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void AZClientGameControllerBase::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void AZClientGameControllerBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZClientGameControllerBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world && world->IsGameWorld())
	{
		ExtensionScope.Open(Create(this, "ZeroClientGameControllerExtensionScope"));
		CommonGameZ::ZClientGameControllerSubsystemBase_Private::FZSetController { this };

		if (ExtenderResolver)
		{
			if (ExtenderCollection = ExtenderResolver->Resolve(this); ExtenderCollection)
			{
				ExtenderCollection->Register(this);
			}
		}
	}

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZClientGameControllerBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}

void AZClientGameControllerBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);

	if (ExtenderCollection)
	{
		ExtenderCollection->Unregister();
	}

	ExtensionScope.Close();
	
	Super::EndPlay(endPlayReason);
}
