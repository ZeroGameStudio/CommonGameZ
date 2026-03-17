// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"
#include "GameFramework/PlayerState.h"

#include "ZPlayerStateBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZPlayerStateBase : public APlayerState, public IZSharpReplicatedObject
{
	GENERATED_BODY()

public:
	AZPlayerStateBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
};


