// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public readonly record struct CheaterRegistration : IDisposable
{
    public CheaterRegistration(ICheatEngine engine, uint64 handle)
    {
        _engine = engine;
        _handle = handle;
    }
    
    public bool IsValid => _engine is not null;

    private readonly ICheatEngine? _engine;
    private readonly uint64 _handle;
    
    #region IDisposable Implementations

    public void Dispose() => _engine?.UnregisterCheater(this);

    #endregion
}

public interface ICheatEngine
{
    public static ICheatEngine Instance => CheatEngine.Instance;
    
    object? Cheat(string? command, params IEnumerable<object?> args);
    CheaterRegistration RegisterCheater(ICheater? cheater);
    void UnregisterCheater(CheaterRegistration registration);
    bool GetEnvironmentVariable(string? name, [NotNullWhen(true)] out object? value);
    bool SetEnvironmentVariable(string? name, object? value, int32 priority = 0, string? reason = null);
}


