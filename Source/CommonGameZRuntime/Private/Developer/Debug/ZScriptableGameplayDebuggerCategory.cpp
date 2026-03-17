// Copyright Zero Games. All Rights Reserved.

#include "Developer/Debug/ZScriptableGameplayDebuggerCategory.h"

#include "Developer/Debug/ZGameplayDebuggerDataProviderBase.h"

#if WITH_GAMEPLAY_DEBUGGER

TSharedRef<FGameplayDebuggerCategory> FZScriptableGameplayDebuggerCategory::MakeInstance(FName category)
{
	return MakeShared<FZScriptableGameplayDebuggerCategory>(category);
}

FZScriptableGameplayDebuggerCategory::FZScriptableGameplayDebuggerCategory(FName category)
{
	Category = category;
	
	SetDataPackReplication<decltype(Data)>(&Data);
}

void FZScriptableGameplayDebuggerCategory::CollectData(APlayerController* ownerPC, AActor* debugActor)
{
	FGameplayDebuggerCategory::CollectData(ownerPC, debugActor);
	
	Data.Content = UZGameplayDebuggerDataProviderBase::StaticCollectData(Category, ownerPC, debugActor);
}

void FZScriptableGameplayDebuggerCategory::DrawData(APlayerController* ownerPC, FGameplayDebuggerCanvasContext& canvasContext)
{
	FGameplayDebuggerCategory::DrawData(ownerPC, canvasContext);
	
	if (!Data.Content.IsEmpty())
	{
		canvasContext.Printf(TEXT("%s"), *Data.Content);
	}
}

#endif


