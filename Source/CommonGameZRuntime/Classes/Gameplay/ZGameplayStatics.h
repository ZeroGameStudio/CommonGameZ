// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Kismet/GameplayStatics.h"

#include "ZGameplayStatics.generated.h"

class UFXSystemComponent;
class UFXSystemAsset;

UCLASS(Abstract)
class COMMONGAMEZRUNTIME_API UZGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()
	
	// ================================================= End Singleton =================================================
public:
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static UGameInstance* GetPrimaryGameInstance();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static UWorld* GetPrimaryGameWorld();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static AGameModeBase* GetPrimaryGameMode();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static AGameStateBase* GetPrimaryGameState();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static ULocalPlayer* GetPrimaryLocalPlayer();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static APlayerController* GetPrimaryPlayerController();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static APlayerState* GetPrimaryPlayerState();
	
	UFUNCTION(BlueprintPure, Category = "Singleton")
	static APawn* GetPrimaryPlayerPawn();
	// ================================================= End Singleton =================================================
	
	// ================================================= Begin Networking =================================================
public:
	UFUNCTION(BlueprintPure, Category = "Networking")
	static bool IsServer();
	
	UFUNCTION(BlueprintPure, Category = "Networking")
	static bool IsOnlineServer();
	
	UFUNCTION(BlueprintPure, Category = "Networking")
	static bool IsClient();
	// ================================================= Begin Networking =================================================
	
	// ================================================= Begin FX =================================================
public:
	UFUNCTION(BlueprintCallable, Category = "FX", meta = (WorldContext = "worldContext"))
	static UFXSystemComponent* SpawnVisualEffectAtLocation(UObject* worldContext, UFXSystemAsset* fx, FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = FVector(1));
	
	UFUNCTION(BlueprintCallable, Category = "FX", meta = (WorldContext = "worldContext"))
	static UFXSystemComponent* SpawnVisualEffectAttached(UFXSystemAsset* fx, USceneComponent* component, FName socket = NAME_None, FVector location = FVector::ZeroVector, FRotator rotation = FRotator::ZeroRotator, FVector scale = FVector(1), EAttachLocation::Type locationType = EAttachLocation::KeepRelativeOffset);
	// ================================================= End Networking =================================================
	
};


