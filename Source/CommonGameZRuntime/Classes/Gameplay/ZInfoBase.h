// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"

#include "ZInfoBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZInfoBase : public AInfo, public IZSharpReplicatedObject
{
	GENERATED_BODY()

public:
	AZInfoBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

protected:
	virtual void Tick(float deltaSeconds) override;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
};


