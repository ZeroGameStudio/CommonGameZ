// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZInfoBase.h"

#include "ZExtensibleInfoBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZExtensibleInfoBase : public AZInfoBase
{
	GENERATED_BODY()

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
};


