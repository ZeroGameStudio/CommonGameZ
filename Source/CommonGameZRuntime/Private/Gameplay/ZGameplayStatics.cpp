// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/ZGameplayStatics.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/GameEngine.h"
#include "GameFramework/GameModeBase.h"

UGameInstance* UZGameplayStatics::GetPrimaryGameInstance()
{
	// IMPORTANT: Keep this behavior same as Z# primary game instance.
	UGameInstance* result = nullptr;
	if (auto gameEngine = Cast<const UGameEngine>(GEngine))
	{
		result = gameEngine->GameInstance;
	}
		
#if WITH_EDITOR
	
	else if (auto editorEngine = Cast<const UEditorEngine>(GEngine))
	{
		const FWorldContext* primaryPIEWorld = editorEngine->GetWorldContextFromPIEInstance(0);
		result = primaryPIEWorld ? primaryPIEWorld->OwningGameInstance : nullptr;
	}
	
#endif
	
	return result;
}

UWorld* UZGameplayStatics::GetPrimaryGameWorld()
{
	const UGameInstance* primaryGameInst = GetPrimaryGameInstance();
	if (!primaryGameInst)
	{
		return nullptr;
	}
	
	return primaryGameInst->GetWorld();
}

AGameModeBase* UZGameplayStatics::GetPrimaryGameMode()
{
	const UWorld* primaryGameWorld = GetPrimaryGameWorld();
	if (!primaryGameWorld)
	{
		return nullptr;
	}
	
	return primaryGameWorld->GetAuthGameMode();
}

AGameStateBase* UZGameplayStatics::GetPrimaryGameState()
{
	const AGameModeBase* primaryGameMode = GetPrimaryGameMode();
	if (!primaryGameMode)
	{
		return nullptr;
	}
	
	return primaryGameMode->GameState;
}

ULocalPlayer* UZGameplayStatics::GetPrimaryLocalPlayer()
{
	const UGameInstance* primaryGameInst = GetPrimaryGameInstance();
	if (!primaryGameInst)
	{
		return nullptr;
	}
	
	return primaryGameInst->GetFirstGamePlayer();
}

APlayerController* UZGameplayStatics::GetPrimaryPlayerController()
{
	const UGameInstance* primaryGameInst = GetPrimaryGameInstance();
	if (!primaryGameInst)
	{
		return nullptr;
	}
	
	return primaryGameInst->GetFirstLocalPlayerController();
}

APlayerState* UZGameplayStatics::GetPrimaryPlayerState()
{
	const APlayerController* primaryPlayerController = GetPrimaryPlayerController();
	if (!primaryPlayerController)
	{
		return nullptr;
	}
	
	return primaryPlayerController->PlayerState;
}

APawn* UZGameplayStatics::GetPrimaryPlayerPawn()
{
	const APlayerController* primaryPlayerController = GetPrimaryPlayerController();
	if (!primaryPlayerController)
	{
		return nullptr;
	}
	
	return primaryPlayerController->GetPawn();
}

bool UZGameplayStatics::IsServer()
{
	if (IsRunningDedicatedServer())
	{
		return true;
	}
	
	if (IsRunningClientOnly())
	{
		return false;
	}
	
	const UWorld* primaryGameWorld = GetPrimaryGameWorld();
	if (!primaryGameWorld)
	{
		return false;
	}
	
	return primaryGameWorld->GetNetMode() != NM_Client;
}

bool UZGameplayStatics::IsOnlineServer()
{
	if (IsRunningDedicatedServer())
	{
		return true;
	}
	
	if (IsRunningClientOnly())
	{
		return false;
	}
	
	const UWorld* primaryGameWorld = GetPrimaryGameWorld();
	if (!primaryGameWorld)
	{
		return false;
	}
	
	ENetMode netMode = primaryGameWorld->GetNetMode();
	return netMode != NM_Standalone && netMode != NM_Client;
}

bool UZGameplayStatics::IsClient()
{
	return !IsRunningDedicatedServer();
}

UFXSystemComponent* UZGameplayStatics::SpawnVisualEffectAtLocation(UObject* worldContext, UFXSystemAsset* fx, FVector location, FRotator rotation, FVector scale)
{
	if (auto niagara = Cast<UNiagaraSystem>(fx))
	{
		return UNiagaraFunctionLibrary::SpawnSystemAtLocation(worldContext, niagara, location, rotation, scale);
	}
	else if (auto cascade = Cast<UParticleSystem>(fx))
	{
		return SpawnEmitterAtLocation(worldContext, cascade, location, rotation, scale);
	}
	
	return nullptr;
}

UFXSystemComponent* UZGameplayStatics::SpawnVisualEffectAttached(UFXSystemAsset* fx, USceneComponent* component, FName socket, FVector location, FRotator rotation, FVector scale, EAttachLocation::Type locationType)
{
	if (auto niagara = Cast<UNiagaraSystem>(fx))
	{
		FFXSystemSpawnParameters params{};
		params.SystemTemplate = niagara;
		params.AttachToComponent = component;
		params.AttachPointName = socket;
		params.Location = location;
		params.Rotation = rotation;
		params.Scale = scale;
		params.LocationType = locationType;
		params.bAutoDestroy = true;
		params.bAutoActivate = true;
		params.PoolingMethod = EPSCPoolMethod::None;
		params.bPreCullCheck = true;
		return UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(params);
	}
	else if (auto cascade = Cast<UParticleSystem>(fx))
	{
		return SpawnEmitterAttached(cascade, component, socket, location, rotation, scale, locationType);
	}
	
	return nullptr;
}


