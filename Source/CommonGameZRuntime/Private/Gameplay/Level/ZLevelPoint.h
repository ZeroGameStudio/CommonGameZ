// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagAssetInterface.h"
#include "Gameplay/ZInfoBase.h"

#include "ZLevelPoint.generated.h"

UCLASS()
class COMMONGAMEZRUNTIME_API AZLevelPoint : public AZInfoBase, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	AZLevelPoint();
	
public:
	UFUNCTION(BlueprintGetter)
	int32 GetIndex() const { return Index; }
	
public:
	const FGameplayTagContainer& GetGameplayTags() const { return GameplayTags; }
	
public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& tagContainer) const override { tagContainer = GameplayTags; }
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& outLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated, BlueprintGetter = GetIndex, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 Index;
	
	UPROPERTY(Replicated, EditAnywhere)
	FGameplayTagContainer GameplayTags;
	
};
