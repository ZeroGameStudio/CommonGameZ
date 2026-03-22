// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

[UClass, Abstract, Blueprintable]
public partial class AZScriptPlayerControllerBase : AZPlayerControllerBase
{
    
    [UProperty, DefaultSubobject, BlueprintReadOnly, VisibleAnywhere]
    private partial UZCheatComponent CheatComp { get; set; }
    
}




