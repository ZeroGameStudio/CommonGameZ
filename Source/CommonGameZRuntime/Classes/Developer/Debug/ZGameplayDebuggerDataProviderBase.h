// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGameplayDebuggerDataProviderBase.generated.h"

UCLASS(NotBlueprintable, Config = CommonGameZ, DefaultConfig, MinimalAPI)
class UZGameplayDebuggerDataProviderBase : public UObject
{
	GENERATED_BODY()
	
public:
	static FString StaticCollectData(FName category, APlayerController* ownerPC, AActor* debugActor);
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	FString CollectData(FName category, APlayerController* ownerPC, AActor* debugActor) const;
	
private:
	// Impl class may not exist when creating base CDO, use soft ref.
	UPROPERTY(Config)
	TSoftClassPtr<UZGameplayDebuggerDataProviderBase> ImplementationClass;
	
};


