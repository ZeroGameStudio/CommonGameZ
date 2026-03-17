// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZExtensibleInfoBase.h"

#include "ZActorExtensionHelper.h"

void AZExtensibleInfoBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZExtensibleInfoBase::BeginPlay()
{
	Super::BeginPlay();

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZExtensibleInfoBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);
	
	Super::EndPlay(endPlayReason);
}


