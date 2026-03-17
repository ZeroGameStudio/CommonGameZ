// Copyright Zero Games. All Rights Reserved.

#include "CommonGameZRuntimeModule.h"

class FCommonGameZRuntimeModule : public ICommonGameZRuntimeModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FCommonGameZRuntimeModule, CommonGameZRuntime)

void FCommonGameZRuntimeModule::StartupModule()
{
}

void FCommonGameZRuntimeModule::ShutdownModule()
{
}


