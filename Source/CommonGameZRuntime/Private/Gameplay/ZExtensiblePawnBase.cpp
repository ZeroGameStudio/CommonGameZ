// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZExtensiblePawnBase.h"

#include "ZActorExtensionHelper.h"

void AZExtensiblePawnBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZExtensiblePawnBase::BeginPlay()
{
	Super::BeginPlay();

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZExtensiblePawnBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);
	
	Super::EndPlay(endPlayReason);
}


