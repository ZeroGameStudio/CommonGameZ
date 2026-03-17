// Copyright Zero Games. All Rights Reserved.

#include "Engine/ZGameInstanceBase.h"

#include "CommonGameZRuntimeLogChannels.h"
#include "Resolver/ZExtenderCollection.h"
#include "Resolver/ZExtenderResolverBaseInterface.h"
#include "Scope/ZGameExtensionScope.h"

void UZGameInstanceBase::Init()
{
	Super::Init();

	if (ExtenderResolver)
	{
		if (ExtenderCollection = ExtenderResolver->Resolve(this); ExtenderCollection)
		{
			ExtenderCollection->Register(&UZGameExtensionScope::Get(this));
		}
	}
}

void UZGameInstanceBase::Shutdown()
{
	if (ExtenderCollection)
	{
		ExtenderCollection->Unregister();
	}
	
	Super::Shutdown();
}


