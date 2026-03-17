// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZPlayerControllerBase.h"

#include "EnhancedInputComponent.h"
#include "ZActorExtensionHelper.h"
#include "Emit/IZSharpFieldRegistry.h"

AZPlayerControllerBase::AZPlayerControllerBase()
	: bIsZSharpClass(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(GetClass()))
	, bHasZSharpTick(ZSharp::IZSharpFieldRegistry::Get().IsZSharpClass(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ReceiveTick))->GetOuterUClass()))
{
	bShowMouseCursor = true;
}

void AZPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(outLifetimeProps);

	ZSharpReplicatedObject_GetLifetimeReplicatedProps(outLifetimeProps);
}

void AZPlayerControllerBase::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void AZPlayerControllerBase::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void AZPlayerControllerBase::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void AZPlayerControllerBase::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void AZPlayerControllerBase::AddHiddenActor(AActor* actor)
{
	if (!actor)
	{
		return;
	}

	int32 oldNum = HiddenActors.Num();
	HiddenActors.AddUnique(actor);
	if (oldNum != HiddenActors.Num())
	{
		actor->MarkComponentsRenderStateDirty();
	}
}

void AZPlayerControllerBase::RemoveHiddenActor(AActor* actor)
{
	if (!actor)
	{
		return;
	}
	
	if (HiddenActors.RemoveAllSwap([actor](TObjectPtr<AActor> elem){ return elem == actor; }))
	{
		actor->MarkComponentsRenderStateDirty();
	}
}

void AZPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto component = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		ScriptSetupInputComponent(component);
	}
}

void AZPlayerControllerBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	CommonGameZ::FZActorExtensionHelper::RegisterInitializeComponentsChannel(this);
}

void AZPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world && world->IsGameWorld())
	{
		FString name = "ZeroPlayerControllerExtensionScope - ";
		name.Append(GetName());
		ExtensionScope.Open(Create(this, FName { name }));
	}

	CommonGameZ::FZActorExtensionHelper::RegisterBeginPlayChannel(this);
}

void AZPlayerControllerBase::Tick(float deltaSeconds)
{
	if (bHasZSharpTick)
	{
		ReceiveTick(deltaSeconds);

		// No need to process latent actions for native class.
	}
	
	Super::Tick(deltaSeconds);
}

void AZPlayerControllerBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	CommonGameZ::FZActorExtensionHelper::UnregisterOnEndPlay(this);
	
	ExtensionScope.Close();
	
	Super::EndPlay(endPlayReason);
}

void AZPlayerControllerBase::OnPossess(APawn* pawn)
{
	FRotator stashedControlRotation = GetControlRotation();
	Super::OnPossess(pawn);
	if (!bSnapToPawnRotationOnPossess)
	{
		SetControlRotation(stashedControlRotation);
	}
}


