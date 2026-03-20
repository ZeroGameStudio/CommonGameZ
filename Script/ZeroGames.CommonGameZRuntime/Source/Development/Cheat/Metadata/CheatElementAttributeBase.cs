// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public abstract class CheatElementAttributeBase : Attribute
{
    public string? Name { get; init; }
    public string? Desc { get; init; }
    public string? Help { get; init; }
}


