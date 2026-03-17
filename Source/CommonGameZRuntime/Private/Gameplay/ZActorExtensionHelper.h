// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

namespace CommonGameZ
{
	struct FZActorExtensionHelper
	{
		static void RegisterInitializeComponentsChannel(AActor* extendee);
		static void RegisterBeginPlayChannel(AActor* extendee);
		static void UnregisterOnEndPlay(AActor* extendee);

	private:
		static bool IsGameActor(AActor* extendee, UWorld*& world);
		static void RegisterExtendee(AActor* extendee, FGameplayTag channel, UWorld* world);
	};
}


