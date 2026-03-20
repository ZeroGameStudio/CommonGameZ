// Copyright Zero Games. All Rights Reserved.

using System.Text.RegularExpressions;

namespace ZeroGames.CommonGameZRuntime;

[UClass, Abstract]
public partial class UZrpgCheatBlueprintLibrary : UBlueprintFunctionLibrary
{

    [UFunction, BlueprintCallable]
    public static void Cheat(FString? command)
    {
        if (command is null)
        {
            return;
        }
        
        string[] tokens = Regex.Replace(command.Data, @"\s+", " ").Split(' ', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
        if (tokens.Length < 1)
        {
            return;
        }

        string name = tokens[0];
        string[] args = tokens[1..];
        
        ICheatEngine.Instance.Cheat(name, args);
    }
    
}


