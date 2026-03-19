// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

[UClass, Abstract, NotBlueprintable]
public partial class UZScriptGameWorldSubsystemBase : UZScriptWorldSubsystemBase
{
	#region Overrides

	protected sealed override bool ScriptDoesSupportWorldType_Implementation(int32 worldType)
	{
		return (EWorldType)worldType is EWorldType.Game or EWorldType.PIE;
	}

	#endregion
}


