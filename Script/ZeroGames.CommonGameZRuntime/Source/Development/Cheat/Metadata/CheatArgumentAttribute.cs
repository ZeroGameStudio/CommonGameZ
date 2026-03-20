// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

[AttributeUsage(AttributeTargets.Parameter)]
public class CheatArgumentAttribute : CheatElementAttributeBase
{
    public string? EnvironmentVariable { get; set; }
}


