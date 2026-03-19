// Copyright Zero Games. All Rights Reserved.


#include "ZLevelPoint.h"

#include "Net/UnrealNetwork.h"

AZLevelPoint::AZLevelPoint()
{
	bAlwaysRelevant = true;
	
	USceneComponent* oldRoot = RootComponent;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	if (oldRoot)
	{
		oldRoot->SetupAttachment(RootComponent);
	}
}

void AZLevelPoint::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);
	
	FDoRepLifetimeParams params;
	params.bIsPushBased = true;
	{
		auto& OutLifetimeProps = outLifetimeProps;
		DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, Index, params);
	}
}


