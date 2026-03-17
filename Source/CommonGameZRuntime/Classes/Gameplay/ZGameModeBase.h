// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "Scope/ZDeferredExtensionScope.h"
#include "Scope/ZExtensionScope.h"

#include "ZGameModeBase.generated.h"

class UZExtenderResolverBaseInterface;
class IZExtenderCollection;
class AZClientGameControllerBase;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZGameModeBase : public AGameModeBase, public IZExtensionScope
{
	GENERATED_BODY()

public:
	AZGameModeBase();

public:
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void InitGameState() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* newPlayer, const FTransform& spawnTransform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	APawn* ScriptSpawnDefaultPawnAtTransform(AController* newPlayer, const FTransform& spawnTransform, bool& succeed);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes, meta = (AllowPrivateAccess))
	TSubclassOf<AZClientGameControllerBase> ClientGameControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Extension")
	TObjectPtr<UZExtenderResolverBaseInterface> ExtenderResolver;

	UPROPERTY(Transient)
	TScriptInterface<IZExtenderCollection> ExtenderCollection;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


