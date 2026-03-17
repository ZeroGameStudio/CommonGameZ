// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZClientGameControllerSubsystemBase.h"

#include "CommonGameZRuntimeLogChannels.h"
#include "Gameplay/ZClientGameControllerBase.h"

CommonGameZ::ZClientGameControllerSubsystemBase_Private::FZSetController::FZSetController(AZClientGameControllerBase* controller)
{
	UWorld* world = controller->GetWorld();
	if (!world)
	{
		return;
	}

	auto subsystem = world->GetSubsystem<UZClientGameControllerSubsystemBase>();
	if (!subsystem)
	{
		return;
	}

	subsystem->SetController(controller);
}

void UZClientGameControllerSubsystemBase::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void UZClientGameControllerSubsystemBase::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void UZClientGameControllerSubsystemBase::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZClientGameControllerSubsystemBase::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

bool UZClientGameControllerSubsystemBase::ShouldCreateSubsystem(UObject* outer) const
{
	if (IsRunningDedicatedServer())
	{
		return false;
	}

	auto world = CastChecked<UWorld>(outer);
	return world->IsGameWorld();
}

void UZClientGameControllerSubsystemBase::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ScriptInitialize();
}

void UZClientGameControllerSubsystemBase::Deinitialize()
{
	ScriptDeinitialize();
	
	ExtensionScope.Close();
	
	Super::Deinitialize();
}

void UZClientGameControllerSubsystemBase::SetController(AZClientGameControllerBase* controller)
{
	if (!ensure(!Controller))
	{
		UE_LOG(LogCommonGameZRuntime, Error, TEXT("[UZClientGameControllerSubsystem::SetController] Duplicated ClientGameController instance!"));
		return;
	}
	
	Controller = controller;
	ExtensionScope.Open(Controller.Get());

	ReceivedController();
}


