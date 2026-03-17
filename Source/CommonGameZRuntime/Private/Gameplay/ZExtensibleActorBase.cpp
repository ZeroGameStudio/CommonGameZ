// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZExtensibleActorBase.h"

#include "Gameplay/ZActorExtensionHelper.h"

void AZExtensibleActorBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZExtensibleActorBase::BeginPlay()
{
	Super::BeginPlay();

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZExtensibleActorBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);
	
	Super::EndPlay(endPlayReason);
}


