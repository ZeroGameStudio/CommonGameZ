// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"
#include "Scope/ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZPlayerControllerBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZPlayerControllerBase : public APlayerController, public IZSharpReplicatedObject, public IZExtensionScope
{
	GENERATED_BODY()

public:
	AZPlayerControllerBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Render")
	void AddHiddenActor(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "Render")
	void RemoveHiddenActor(AActor* actor);
	
protected:
	virtual void SetupInputComponent() override;
	
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	virtual void OnPossess(APawn* pawn) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptSetupInputComponent(UEnhancedInputComponent* component);

protected:
	UPROPERTY(EditAnywhere, Category = "Possession")
	bool bSnapToPawnRotationOnPossess = true;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


