// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/Level/ZLevelPoint.h"

#include "Components/ArrowComponent.h"
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
	
#if WITH_EDITORONLY_DATA
	if (auto arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow")))
	{
		arrow->ArrowColor = FColor(150, 200, 255);
		arrow->bTreatAsASprite = true;
		arrow->SpriteInfo.Category = "LevelPoint";
		arrow->SpriteInfo.DisplayName = NSLOCTEXT("CommonGameZRuntime", "LevelPoint", "LevelPoint");
		arrow->SetupAttachment(RootComponent);
		arrow->bIsScreenSizeScaled = true;
		arrow->SetSimulatePhysics(false);
	}
#endif
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


