// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZGameInstanceBase.generated.h"

class UZExtenderResolverBaseInterface;
class IZExtenderCollection;

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	int32 GetNumLocalPlayers() const { return Super::GetNumLocalPlayers(); }

	UFUNCTION(BlueprintPure)
	ULocalPlayer* GetLocalPlayerByIndex(const int32 index) const { return Super::GetLocalPlayerByIndex(index); }

	UFUNCTION(BlueprintPure)
	ULocalPlayer* GetFirstGamePlayer() const { return Super::GetFirstGamePlayer(); }

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Extension")
	TObjectPtr<UZExtenderResolverBaseInterface> ExtenderResolver;

	UPROPERTY(Transient)
	TScriptInterface<IZExtenderCollection> ExtenderCollection;
	
};


