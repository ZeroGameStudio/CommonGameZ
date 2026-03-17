// Copyright Zero Games. All Rights Reserved.

#pragma once

class ICommonGameZRuntimeModule : public IModuleInterface
{
public:
	static FORCEINLINE ICommonGameZRuntimeModule& Get()
	{
		static ICommonGameZRuntimeModule& GSingleton = FModuleManager::LoadModuleChecked<ICommonGameZRuntimeModule>("CommonGameZRuntime");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CommonGameZRuntime");
	}
};


