// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZWorldSubsystemBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZWorldSubsystemBase : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UZWorldSubsystemBase()
		: bHasScriptShouldCreateSubsystem(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptShouldCreateSubsystem))->GetOuterUClass() != StaticClass())
		, bHasScriptDoesSupportWorldType(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptDoesSupportWorldType))->GetOuterUClass() != StaticClass()){}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptShouldCreateSubsystem(UObject* outer) const;

	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptDoesSupportWorldType(int32 worldType) const;

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptPostInitialize();

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnWorldBeginPlay(UWorld* world);

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnWorldComponentsUpdated(UWorld* world);

	UFUNCTION(BlueprintImplementableEvent)
	void ScriptDeinitialize();
	
private:
	virtual bool ShouldCreateSubsystem(UObject* outer) const override { return Super::ShouldCreateSubsystem(outer) && (!bHasScriptShouldCreateSubsystem || ScriptShouldCreateSubsystem(outer)); }
	virtual bool DoesSupportWorldType(const EWorldType::Type worldType) const override { static_assert(std::is_same_v<std::underlying_type_t<EWorldType::Type>, int32>); return bHasScriptDoesSupportWorldType ? ScriptDoesSupportWorldType(worldType) : Super::DoesSupportWorldType(worldType); }
	virtual void Initialize(FSubsystemCollectionBase& collection) override { Super::Initialize(collection); ScriptInitialize(); }
	virtual void PostInitialize() override { Super::PostInitialize(); ScriptPostInitialize(); }
	virtual void OnWorldBeginPlay(UWorld& world) override { Super::OnWorldBeginPlay(world); ScriptOnWorldBeginPlay(&world); }
	virtual void OnWorldComponentsUpdated(UWorld& world) override { Super::OnWorldComponentsUpdated(world); ScriptOnWorldComponentsUpdated(&world); }
	virtual void Deinitialize() override { ScriptDeinitialize(); Super::Deinitialize(); }

private:
	bool bHasScriptShouldCreateSubsystem;
	bool bHasScriptDoesSupportWorldType;
	
};
