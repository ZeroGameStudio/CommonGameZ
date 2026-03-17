// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Scope/ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZClientGameControllerBase.generated.h"

class IZExtenderCollection;
class UZExtenderResolverBaseInterface;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZClientGameControllerBase : public AInfo, public IZExtensionScope
{
	GENERATED_BODY()

public:
	AZClientGameControllerBase();

public:
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Extension")
	TObjectPtr<UZExtenderResolverBaseInterface> ExtenderResolver;

	UPROPERTY(Transient)
	TScriptInterface<IZExtenderCollection> ExtenderCollection;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
	ZES::FZDeferredExtensionScope ExtensionScope;

};


