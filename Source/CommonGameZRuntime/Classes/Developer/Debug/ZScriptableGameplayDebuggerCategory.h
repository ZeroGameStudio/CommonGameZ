// Copyright Zero Games. All Rights Reserved.

#pragma once

#if WITH_GAMEPLAY_DEBUGGER

#include "GameplayDebuggerCategory.h"

class FZScriptableGameplayDebuggerCategory : public FGameplayDebuggerCategory
{
	
public:
	COMMONGAMEZRUNTIME_API static TSharedRef<FGameplayDebuggerCategory> MakeInstance(FName category);
	
public:
	FZScriptableGameplayDebuggerCategory(FName category);
	
	virtual void CollectData(APlayerController* ownerPC, AActor* debugActor) override;
	virtual void DrawData(APlayerController* ownerPC, FGameplayDebuggerCanvasContext& canvasContext) override;
	
private:
	FName Category;
	
	struct FRepData
	{
		void Serialize(FArchive& ar) { ar << Content; }
		FString Content;
	} Data;
	
};

#endif
