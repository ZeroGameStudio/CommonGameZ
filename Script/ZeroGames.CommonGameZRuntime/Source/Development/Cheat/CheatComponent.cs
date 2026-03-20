// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

[UClass, DefaultToTransient, DefaultToReplicated, NotBlueprintable]
public partial class UZrpgCheatComponent : UZActorComponentBase
{

    [UFunction, Server, Reliable, SealedEvent]
    public partial void ServerCheat(FString command, TArray<FString> args);
    private partial void ServerCheat_Implementation(FString command, TArray<FString> args)
    {
        ICheatEngine.Instance.Cheat(command, args.Select(a => a.Data));
    }
    
}


