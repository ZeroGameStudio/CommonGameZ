// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZEngineSubsystemBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZEngineSubsystemBase : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UZEngineSubsystemBase()
		: bHasScriptShouldCreateSubsystem(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptShouldCreateSubsystem))->GetOuterUClass() != StaticClass()){}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptShouldCreateSubsystem(UObject* outer) const;

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptDeinitialize();
	
private:
	virtual bool ShouldCreateSubsystem(UObject* outer) const override { return Super::ShouldCreateSubsystem(outer) && (!bHasScriptShouldCreateSubsystem || ScriptShouldCreateSubsystem(outer)); }
	virtual void Initialize(FSubsystemCollectionBase& collection) override { Super::Initialize(collection); ScriptInitialize(); }
	virtual void Deinitialize() override { ScriptDeinitialize(); Super::Deinitialize(); }

private:
	bool bHasScriptShouldCreateSubsystem;
	
};


