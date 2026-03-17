// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZSharpReplicatedObject.h"

#include "ZActorComponentBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZActorComponentBase : public UActorComponent, public IZSharpReplicatedObject
{
	GENERATED_BODY()

public:
	UZActorComponentBase();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;

public:
	UFUNCTION(BlueprintPure, Category = "Replication")
	ENetRole GetLocalRole() const;

	UFUNCTION(BlueprintPure, Category = "Replication")
	ENetRole GetRemoteRole() const;

	UFUNCTION(BlueprintPure, Category = "Replication")
	bool HasAuthority() const;

protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
	virtual void Activate(bool reset = false) override;
	virtual void Deactivate() override;
	
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveActivate();

	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveDeactivate();
	
private:
	bool bIsZSharpClass;
	bool bHasZSharpTick;
	
};


