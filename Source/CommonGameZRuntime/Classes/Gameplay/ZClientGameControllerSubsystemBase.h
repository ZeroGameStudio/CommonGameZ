// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Scope/ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZClientGameControllerSubsystemBase.generated.h"

class AZClientGameControllerBase;

namespace CommonGameZ::ZClientGameControllerSubsystemBase_Private
{
	struct FZSetController
	{
		friend class AZClientGameControllerBase;
	private:
		FZSetController(AZClientGameControllerBase* controller);
	};
}

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZClientGameControllerSubsystemBase : public UWorldSubsystem, public IZExtensionScope
{
	GENERATED_BODY()

	friend CommonGameZ::ZClientGameControllerSubsystemBase_Private::FZSetController;

public:
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

public:
	UFUNCTION(BlueprintPure)
	AZClientGameControllerBase* GetController() const { return Controller; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptDeinitialize();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ReceivedController();

private:
	virtual bool ShouldCreateSubsystem(UObject* outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	void SetController(AZClientGameControllerBase* controller);

private:
	UPROPERTY()
	TObjectPtr<AZClientGameControllerBase> Controller;

private:
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


