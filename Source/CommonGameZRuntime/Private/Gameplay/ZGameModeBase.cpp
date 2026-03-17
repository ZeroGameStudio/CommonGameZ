// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZGameModeBase.h"

#include "ZActorExtensionHelper.h"
#include "Emit/IZSharpFieldRegistry.h"
#include "Gameplay/ZGameStateBase.h"
#include "Resolver/ZExtenderCollection.h"
#include "Resolver/ZExtenderResolverBaseInterface.h"

AZGameModeBase::AZGameModeBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{

}

void AZGameModeBase::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void AZGameModeBase::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void AZGameModeBase::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void AZGameModeBase::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void AZGameModeBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZGameModeBase::InitGameState()
{
	Super::InitGameState();

	if (GetWorld()->IsGameWorld())
	{
		CommonGameZ::ZGameStateBase_Private::FZSetClientGameControllerClass(CastChecked<AZGameStateBase>(GameState), ClientGameControllerClass);
	}
}

APawn* AZGameModeBase::SpawnDefaultPawnAtTransform_Implementation(AController* newPlayer, const FTransform& spawnTransform)
{
	bool suc;
	APawn* scriptPawn = ScriptSpawnDefaultPawnAtTransform(newPlayer, spawnTransform, suc);
	if (suc)
	{
		return scriptPawn;
	}

	return Super::SpawnDefaultPawnAtTransform_Implementation(newPlayer, spawnTransform);
}

void AZGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world && world->IsGameWorld())
	{
		ExtensionScope.Open(Create(this, "ZeroGameModeExtensionScope"));

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

void AZGameModeBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}

void AZGameModeBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);

	if (ExtenderCollection)
	{
		ExtenderCollection->Unregister();
	}

	ExtensionScope.Close();
	
	Super::EndPlay(endPlayReason);
}


