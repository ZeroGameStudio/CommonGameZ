// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZActorBase.h"

#include "ZExtensibleActorBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZExtensibleActorBase : public AZActorBase
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
};


