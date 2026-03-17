// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"
#include "GameFramework/GameStateBase.h"

#include "ZGameStateBase.generated.h"

class AZGameStateBase;
class AZClientGameControllerBase;

namespace CommonGameZ::ZGameStateBase_Private
{
	struct FZSetClientGameControllerClass
	{
		friend class AZGameModeBase;
	private:
		FZSetClientGameControllerClass(AZGameStateBase* gameState, TSubclassOf<AZClientGameControllerBase> controllerClass);
	};
}

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZGameStateBase : public AGameStateBase, public IZSharpReplicatedObject
{
	GENERATED_BODY()

	friend CommonGameZ::ZGameStateBase_Private::FZSetClientGameControllerClass;

public:
	AZGameStateBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

protected:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

private:
	UFUNCTION()
	void OnRep_ClientGameControllerClass();

private:
	void SetClientGameControllerClass(TSubclassOf<AZClientGameControllerBase> controllerClass);
	void ReceivedClientGameControllerClass();

private:
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ClientGameControllerClass, VisibleInstanceOnly, Category = "GameState")
	TSubclassOf<AZClientGameControllerBase> ClientGameControllerClass;

private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
};


