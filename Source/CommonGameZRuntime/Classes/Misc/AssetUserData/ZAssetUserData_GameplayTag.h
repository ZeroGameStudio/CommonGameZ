// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/AssetUserData.h"
#include "GameplayTagContainer.h"

#include "ZAssetUserData_GameplayTag.generated.h"

UCLASS(BlueprintType, MinimalAPI)
class UZAssetUserData_GameplayTag : public UAssetUserData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Tags;
	
};


