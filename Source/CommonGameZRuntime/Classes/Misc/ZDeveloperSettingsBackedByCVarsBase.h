// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettingsBackedByCVars.h"

#include "ZDeveloperSettingsBackedByCVarsBase.generated.h"

/**
 * IMPORTANT: If you make changes to this class, mirror to UZDeveloperSettings.
 */
UCLASS(Abstract, NotBlueprintable)
class COMMONGAMEZRUNTIME_API UZDeveloperSettingsBackedByCVarsBase : public UDeveloperSettingsBackedByCVars
{
	GENERATED_BODY()
	
public:
	UZDeveloperSettingsBackedByCVarsBase()
		: bHasScriptGetContainerName(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptGetContainerName))->GetOuterUClass() != StaticClass())
		, bHasScriptGetCategoryName(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptGetCategoryName))->GetOuterUClass() != StaticClass())
		, bHasScriptGetSectionName(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptGetSectionName))->GetOuterUClass() != StaticClass())
		, bHasScriptGetSectionText(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptGetSectionText))->GetOuterUClass() != StaticClass())
		, bHasScriptGetSectionDescription(FindFunctionChecked(GET_FUNCTION_NAME_CHECKED(ThisClass, ScriptGetSectionDescription))->GetOuterUClass() != StaticClass())
	{
		CategoryName = "ZeroGames";
	}

protected:
	UFUNCTION(BlueprintImplementableEvent)
	FName ScriptGetContainerName() const;

	UFUNCTION(BlueprintImplementableEvent)
	FName ScriptGetCategoryName() const;

	UFUNCTION(BlueprintImplementableEvent)
	FName ScriptGetSectionName() const;

	UFUNCTION(BlueprintImplementableEvent)
	FText ScriptGetSectionText() const;

	UFUNCTION(BlueprintImplementableEvent)
	FText ScriptGetSectionDescription() const;
	
private:
	virtual FName GetContainerName() const override { return bHasScriptGetContainerName ? ScriptGetContainerName() : Super::GetContainerName(); }
	virtual FName GetCategoryName() const override { return bHasScriptGetCategoryName ? ScriptGetCategoryName() : Super::GetCategoryName(); }
	virtual FName GetSectionName() const override { return bHasScriptGetSectionName ? ScriptGetSectionName() : Super::GetSectionName(); }

#if WITH_EDITOR
	virtual FText GetSectionText() const override { return bHasScriptGetSectionText ? Super::GetSectionText() : Super::GetSectionText(); }
	virtual FText GetSectionDescription() const override { return bHasScriptGetSectionDescription ? Super::GetSectionDescription() : Super::GetSectionDescription(); }
#endif

private:
	bool bHasScriptGetContainerName;
	bool bHasScriptGetCategoryName;
	bool bHasScriptGetSectionName;
	bool bHasScriptGetSectionText;
	bool bHasScriptGetSectionDescription;
	
};
