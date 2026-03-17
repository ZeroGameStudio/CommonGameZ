// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"

#include "ZActorBase.generated.h"

class IZActorMovementReplicationHandler;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZActorBase : public AActor, public IZSharpReplicatedObject
{
	GENERATED_BODY()

public:
	AZActorBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;
	virtual void PostNetReceiveLocationAndRotation() override;
	virtual void PostNetReceiveVelocity(const FVector& newVelocity) override;
	virtual void PostNetReceivePhysicState() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

private:
	UPROPERTY(Transient)
	TScriptInterface<IZActorMovementReplicationHandler> MovementReplicationHandler;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
};


