// Copyright Zero Games. All Rights Reserved.

#include "Developer/Debug/ZGameplayDebuggerDataProviderBase.h"

FString UZGameplayDebuggerDataProviderBase::StaticCollectData(FName category, APlayerController* ownerPC, AActor* debugActor)
{
	const UClass* implementationClass = GetDefault<ThisClass>()->ImplementationClass.LoadSynchronous();
	if (!implementationClass)
	{
		return "{red}PROVIDER NOT FOUND\n";
	}
	
	return GetDefault<ThisClass>(implementationClass)->CollectData(category, ownerPC, debugActor);
}


