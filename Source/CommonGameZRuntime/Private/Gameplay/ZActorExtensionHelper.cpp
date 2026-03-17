// Copyright Zero Games. All Rights Reserved.

#include "ZActorExtensionHelper.h"

#include "GameFramework/GameModeBase.h"
#include "Gameplay/ZClientGameControllerSubsystemBase.h"
#include "Gameplay/ZActorExtensionChannels.h"
#include "Scope/ZEngineExtensionScope.h"
#include "Scope/ZGameExtensionScope.h"
#include "Scope/ZWorldExtensionScope.h"

void CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(AActor* extendee)
{
	UWorld* world;
	if (!IsGameActor(extendee, world))
	{
		return;
	}

	RegisterExtendee(extendee, TAG_ExtensionChannel_ActorInitializeComponents, world);
}

void CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(AActor* extendee)
{
	UWorld* world;
	if (!IsGameActor(extendee, world))
	{
		return;
	}

	RegisterExtendee(extendee, TAG_ExtensionChannel_ActorBeginPlay, world);
}

void CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(AActor* extendee)
{
	if (extendee->IsUnreachable())
	{
		return;
	}
	
	UWorld* world;
	if (!IsGameActor(extendee, world))
	{
		return;
	}

	if (auto clientGameControllerScope = world->GetSubsystem<UZClientGameControllerSubsystemBase>())
	{
		clientGameControllerScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorBeginPlay);
		clientGameControllerScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorInitializeComponents);
	}
	
	if (auto gameModeScope = Cast<IZExtensionScope>(world->GetAuthGameMode()))
	{
		gameModeScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorBeginPlay);
		gameModeScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorInitializeComponents);
	}

	if (auto worldScope = world->GetSubsystem<UZWorldExtensionScope>())
	{
		worldScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorBeginPlay);
		worldScope->ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorInitializeComponents);
	}

	if (UGameInstance* gameInstance = world->GetGameInstance())
	{
		if (gameInstance->GetWorldContext())
		{
			UZGameExtensionScope::Get(gameInstance).ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorBeginPlay);
			UZGameExtensionScope::Get(gameInstance).ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorInitializeComponents);
		}
	}

	UZEngineExtensionScope::Get().ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorBeginPlay);
	UZEngineExtensionScope::Get().ExtensionScope_UnregisterExtendee(extendee, true, TAG_ExtensionChannel_ActorInitializeComponents);
}

bool CommonGameZ::FZActorExtensionHelper::IsGameActor(AActor* extendee, UWorld*& world)
{
	world = nullptr;
	
	if (extendee->IsTemplate())
	{
		return false;
	}
	
	world = extendee->GetWorld();
	if (!world || !world->IsGameWorld())
	{
		return false;
	}

	return true;
}

void CommonGameZ::FZActorExtensionHelper::RegisterExtendee(AActor* extendee, FGameplayTag channel, UWorld* world)
{
	UZEngineExtensionScope::Get().ExtensionScope_RegisterExtendee(extendee, channel);

	if (UGameInstance* gameInstance = world->GetGameInstance())
	{
		UZGameExtensionScope::Get(gameInstance).ExtensionScope_RegisterExtendee(extendee, channel);
	}

	if (auto worldScope = world->GetSubsystem<UZWorldExtensionScope>())
	{
		worldScope->ExtensionScope_RegisterExtendee(extendee, channel);
	}
	
	if (auto gameModeScope = Cast<IZExtensionScope>(world->GetAuthGameMode()))
	{
		gameModeScope->ExtensionScope_RegisterExtendee(extendee, channel);
	}

	if (auto clientGameControllerScope = world->GetSubsystem<UZClientGameControllerSubsystemBase>())
	{
		clientGameControllerScope->ExtensionScope_RegisterExtendee(extendee, channel);
	}
}


