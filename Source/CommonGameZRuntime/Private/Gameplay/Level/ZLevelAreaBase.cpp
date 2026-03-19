// Copyright Zero Games. All Rights Reserved.

#include "ZLevelAreaBase.h"

#include "EngineUtils.h"
#include "ZLevelPoint.h"
#include "Engine/LevelStreamingDynamic.h"

AZLevelAreaBase::AZLevelAreaBase()
{
	bAlwaysRelevant = true;
	
	USceneComponent* oldRoot = RootComponent;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	if (oldRoot)
	{
		oldRoot->SetupAttachment(RootComponent);
	}
}

void AZLevelAreaBase::Activate()
{
	for (const auto& levelStreaming : LevelStreamings)
	{
		if (::IsValid(levelStreaming))
		{
			levelStreaming->SetShouldBeVisible(true);
		}
	}
}

void AZLevelAreaBase::Deactivate()
{
	for (const auto& levelStreaming : LevelStreamings)
	{
		if (::IsValid(levelStreaming))
		{
			levelStreaming->SetShouldBeVisible(false);
		}
	}
}

AZLevelPoint* AZLevelAreaBase::GetPoint(int32 index, bool evenIfInvisible) const
{
	const TObjectPtr<AZLevelPoint>* point = (evenIfInvisible ? AllPoints : VisiblePoints).Find(index);
	return point && ::IsValid(*point) ? *point : nullptr;
}

void AZLevelAreaBase::GetPoints(TArray<AZLevelPoint*>& points, bool evenIfInvisible) const
{
	points.Reset();
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			points.Emplace(point);
		}
	}
}

AZLevelPoint* AZLevelAreaBase::GetPointByTag(FName tag, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point) && point->Tags.Contains(tag))
		{
			return point;
		}
	}
	
	return nullptr;
}

void AZLevelAreaBase::GetPointsByTag(FName tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible) const
{
	points.Reset();
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point) && point->Tags.Contains(tag))
		{
			points.Emplace(point);
		}
	}
}

AZLevelPoint* AZLevelAreaBase::GetPointByGameplayTag(FGameplayTag tag, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().HasTag(tag))
			{
				return point;
			}
		}
	}
	
	return nullptr;
}

void AZLevelAreaBase::GetPointsByGameplayTag(FGameplayTag tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().HasTag(tag))
			{
				points.Emplace(point);
			}
		}
	}
}

AZLevelPoint* AZLevelAreaBase::GetPointByGameplayTagExact(FGameplayTag tag, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().HasTagExact(tag))
			{
				return point;
			}
		}
	}
	
	return nullptr;
}

void AZLevelAreaBase::GetPointsByGameplayTagExact(FGameplayTag tag, TArray<AZLevelPoint*>& points, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().HasTagExact(tag))
			{
				points.Emplace(point);
			}
		}
	}
}

AZLevelPoint* AZLevelAreaBase::GetPointByGameplayTagQuery(const FGameplayTagQuery& query, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().MatchesQuery(query))
			{
				return point;
			}
		}
	}
	
	return nullptr;
}

void AZLevelAreaBase::GetPointsByGameplayTagQuery(const FGameplayTagQuery& query, TArray<AZLevelPoint*>& points, bool evenIfInvisible) const
{
	for (const auto& [_, point] : evenIfInvisible ? AllPoints : VisiblePoints)
	{
		if (::IsValid(point))
		{
			if (point->GetGameplayTags().MatchesQuery(query))
			{
				points.Emplace(point);
			}
		}
	}
}

void AZLevelAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO: Replication.
	ensure(HasAuthority());
	
	TArray<FString> paths;
	GetLevelInstancePaths(paths);
	for (const auto& path : paths)
	{
		bool success;
		ULevelStreamingDynamic* levelStreaming = ULevelStreamingDynamic::LoadLevelInstance(this, path, GetActorLocation(), GetActorRotation(), success);
		if (success && levelStreaming)
		{
			levelStreaming->OnLevelLoaded.AddUniqueDynamic(this, &ThisClass::HandleLevelLoaded);
			levelStreaming->OnLevelUnloaded.AddUniqueDynamic(this, &ThisClass::HandleLevelUnloaded);
			levelStreaming->OnLevelShown.AddUniqueDynamic(this, &ThisClass::HandleLevelShown);
			levelStreaming->OnLevelHidden.AddUniqueDynamic(this, &ThisClass::HandleLevelHidden);
			LevelStreamings.Emplace(levelStreaming);
		}
	}
}

void AZLevelAreaBase::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	for (const auto& levelStreaming : LevelStreamings)
	{
		if (::IsValid(levelStreaming))
		{
			levelStreaming->OnLevelLoaded.RemoveAll(this);
			levelStreaming->OnLevelUnloaded.RemoveAll(this);
			levelStreaming->OnLevelShown.RemoveAll(this);
			levelStreaming->OnLevelHidden.RemoveAll(this);
			levelStreaming->SetShouldBeLoaded(false);
		}
	}
	
	Super::EndPlay(endPlayReason);
}

void AZLevelAreaBase::InvalidateCache()
{
	AllPoints.Reset();
	VisiblePoints.Reset();
	
	TSet<const ULevel*> relevantLevels;
	for (const auto& levelStreaming : LevelStreamings)
	{
		if (!::IsValid(levelStreaming))
		{
			continue;
		}
		
		const ULevel* level = levelStreaming->GetLoadedLevel();
		if (!level)
		{
			continue;
		}
		
		relevantLevels.Emplace(level);
	}
	
	for (TActorIterator<AZLevelPoint> it { GetWorld() }; it; ++it)
	{
		AZLevelPoint* point = *it;
		const ULevel* level = point->GetLevel();
		if (level && relevantLevels.Contains(level))
		{
			int32 index = point->GetIndex();
			AllPoints.Emplace(index, point);
			if (level->bIsVisible)
			{
				VisiblePoints.Emplace(index, point);
			}
		}
	}
}

void AZLevelAreaBase::HandleLevelLoaded()
{
	InvalidateCache();
}

void AZLevelAreaBase::HandleLevelUnloaded()
{
	InvalidateCache();
}

void AZLevelAreaBase::HandleLevelShown()
{
	InvalidateCache();
}

void AZLevelAreaBase::HandleLevelHidden()
{
	InvalidateCache();
}


