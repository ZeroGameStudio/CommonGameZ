// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZLocalPlayerSubsystemBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZLocalPlayerSubsystemBase : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	UZLocalPlayerSubsystemBase()
		: bHasScriptShouldCreateSubsystem(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptShouldCreateSubsystem))->GetOuterUClass() != StaticClass()){}

public:
	UFUNCTION(BlueprintPure)
	ULocalPlayer* GetLocalPlayer() const { return Super::GetLocalPlayer(); }

	UFUNCTION(BlueprintPure)
	APlayerController* GetLocalPlayerController() const { return GetLocalPlayer()->PlayerController; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptShouldCreateSubsystem(UObject* outer) const;

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptPlayerControllerChanged(APlayerController* newPlayerController);

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptDeinitialize();
	
private:
	virtual bool ShouldCreateSubsystem(UObject* outer) const override { return Super::ShouldCreateSubsystem(outer) && (!bHasScriptShouldCreateSubsystem || ScriptShouldCreateSubsystem(outer)); }
	virtual void Initialize(FSubsystemCollectionBase& collection) override { Super::Initialize(collection); ScriptInitialize(); }
	virtual void PlayerControllerChanged(APlayerController* newPlayerController) override { Super::PlayerControllerChanged(newPlayerController); ScriptPlayerControllerChanged(newPlayerController); }
	virtual void Deinitialize() override { ScriptDeinitialize(); Super::Deinitialize(); }

private:
	bool bHasScriptShouldCreateSubsystem;
	
};
