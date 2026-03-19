// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Gameplay/ZInfoBase.h"

#include "ZLevelAreaBase.generated.h"

class ULevelStreamingDynamic;
class AZLevelPoint;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API AZLevelAreaBase : public AZInfoBase
{
	GENERATED_BODY()
	
public:
	AZLevelAreaBase();
	
public:
	UFUNCTION(BlueprintCallable)
	void Activate();
	
	UFUNCTION(BlueprintCallable)
	void Deactivate();
	
public:
	UFUNCTION(BlueprintPure)
	AZLevelPoint* GetPoint(int32 index, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintCallable)
	void GetPoints(TArray<AZLevelPoint*>& points, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintPure)
	AZLevelPoint* GetPointByTag(FName tag, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintCallable)
	void GetPointsByTag(FName tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintPure)
	AZLevelPoint* GetPointByGameplayTag(FGameplayTag tag, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintCallable)
	void GetPointsByGameplayTag(FGameplayTag tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintPure)
	AZLevelPoint* GetPointByGameplayTagExact(FGameplayTag tag, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintCallable)
	void GetPointsByGameplayTagExact(FGameplayTag tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintPure)
	AZLevelPoint* GetPointByGameplayTagQuery(const FGameplayTagQuery& query, bool evenIfInvisible = true) const;
	
	UFUNCTION(BlueprintCallable)
	void GetPointsByGameplayTagQuery(const FGameplayTagQuery& query, TArray<AZLevelPoint*>& points, bool evenIfInvisible = true) const;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GetLevelInstancePaths(TArray<FString>& paths) const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
private:
	void InvalidateCache();
	
private:
	UFUNCTION()
	void HandleLevelLoaded();
	
	UFUNCTION()
	void HandleLevelUnloaded();
	
	UFUNCTION()
	void HandleLevelShown();
	
	UFUNCTION()
	void HandleLevelHidden();
	
private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<ULevelStreamingDynamic>> LevelStreamings;
	
	UPROPERTY(Transient)
	TMap<int32, TObjectPtr<AZLevelPoint>> AllPoints;
	
	UPROPERTY(Transient)
	TMap<int32, TObjectPtr<AZLevelPoint>> VisiblePoints;
	
};


