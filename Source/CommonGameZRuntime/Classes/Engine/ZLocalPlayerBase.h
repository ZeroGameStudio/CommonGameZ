// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZLocalPlayerBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZLocalPlayerBase : public ULocalPlayer
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerChangedMCDelegate, APlayerController*, newPC);
	
public:
	UPROPERTY()
	FOnPlayerControllerChangedMCDelegate OnPlayerControllerChanged;

protected:
	virtual void ReceivedPlayerController(APlayerController* newPC) override;
	
};


