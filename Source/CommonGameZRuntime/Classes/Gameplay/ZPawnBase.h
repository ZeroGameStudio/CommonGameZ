// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"

#include "ZPawnBase.generated.h"

class IZActorMovementReplicationHandler;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZPawnBase : public APawn, public IZSharpReplicatedObject
{
	GENERATED_BODY()

public:
	AZPawnBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;
	virtual void PostNetReceiveLocationAndRotation() override;
	virtual void PostNetReceiveVelocity(const FVector& newVelocity) override;
	virtual void PostNetReceivePhysicState() override;
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

	virtual void PossessedBy(AController* newController) override;
	virtual void UnPossessed() override;
	virtual void DetachFromControllerPendingDestroy() override;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptSetupInputComponent(UEnhancedInputComponent* component);

private:
	void UnregisterExtendeeFromController(bool destroying);

private:
	UPROPERTY(Transient)
	TScriptInterface<IZActorMovementReplicationHandler> MovementReplicationHandler;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
	bool bPendingDestroy;
	
};


