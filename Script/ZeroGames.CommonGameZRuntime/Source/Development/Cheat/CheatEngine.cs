// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;
using System.Reflection;
using System.Runtime.Loader;

namespace ZeroGames.CommonGameZRuntime;

public class CheatEngine : ICheatEngine
{
    
    public static readonly CheatEngine Instance = new();
    
    private abstract class ElementBase
    {
        public required string Name { get; init; }
        public required string? Desc { get; init; }
        public required string? Help { get; init; }
        public required int32 Priority { get; init; }
        public required ICheater? Cheater { get; init; }
    }

    private class Command : ElementBase
    {
        public required MethodInfo Method { get; init; }
    }
    
    private class Variable : ElementBase
    {
        public required PropertyInfo Property { get; init; }
    }

    private CheatEngine()
    {
        if (IMasterAssemblyLoadContext.Instance is { } alc)
        {
            foreach (var assembly in alc.To<AssemblyLoadContext>().Assemblies)
            {
                LootGlobalCheaters(assembly);
            }

            alc.OnAssemblyLoaded += LootGlobalCheaters;
        }
        
        _ = new AutoConsoleCommand("ZRPG.Cheat", "Run cheat command.", command => Cheat(command[0], command[1..]));
        _ = new AutoConsoleCommand("ZRPG.AuthCheat", "Run cheat command on authority.", command => AuthorityCheat(command[0], command[1..]));
    }
    
    private void LootGlobalCheaters(Assembly assembly)
    {
        foreach (var cheater in assembly.GetTypes().Where(type => type.GetCustomAttribute<CheaterAttribute>() is not null))
        {
            LootCheater(cheater, null);
        }
    }

    private void LootCheater(Type type, ICheater? instance)
    {
        BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static;
        if (instance is not null)
        {
            flags |= BindingFlags.Instance;
        }
        
        foreach (var method in type.GetMethods(flags))
        {
            if (method.GetCustomAttribute<CheatCommandAttribute>() is { } cheatCommandAttribute)
            {
                string name = !string.IsNullOrWhiteSpace(cheatCommandAttribute.Name) ? cheatCommandAttribute.Name : method.Name;
                string key = name.ToLower();
                if (!_commands.TryGetValue(key, out var queue))
                {
                    queue = new();
                    _commands[key] = queue;
                }
                    
                queue.Enqueue(new()
                {
                    Name = name,
                    Desc = cheatCommandAttribute.Desc,
                    Help = cheatCommandAttribute.Help,
                    Priority = cheatCommandAttribute.Priority,
                    Cheater = instance,
                    Method = method,
                }, cheatCommandAttribute.Priority);
            }
                
            // TODO: Var
        }
    }

    private object? Cheat(Command command, params IEnumerable<object?> args)
    {
        ParameterInfo[] parameterInfos = command.Method.GetParameters();
        object?[] arguments = new object?[parameterInfos.Length];
        
        int32 i = 0;
        int32 argNum = 0;
        foreach (var arg in args)
        {
            if (!arguments.IsValidIndex(i))
            {
                break;
            }

            arguments[i] = arg;
            
            ++i;
            ++argNum;
        }
        
        for (i = 0; i < parameterInfos.Length; ++i)
        {
            ParameterInfo parameterInfo = parameterInfos[i];
            Type parameterType = parameterInfo.ParameterType;
            object? argument = arguments[i];
            if (i >= argNum)
            {
                if (parameterInfo.HasDefaultValue)
                {
                    arguments[i] = parameterInfo.DefaultValue;
                }
            }
            else if (argument is null)
            {
                if (parameterType.IsValueType && Nullable.GetUnderlyingType(parameterType) is null)
                {
                    arguments[i] = Activator.CreateInstance(parameterType);
                }
            }
            else
            {
                Type argumentType = argument.GetType();
                if (!argumentType.IsAssignableTo(parameterType))
                {
                    // FIXME: 支持扩展
                    if (argument is string stringArgument)
                    {
                        Type parsableType = typeof(IParsable<>).MakeGenericType(parameterType);
                        if (parameterType.IsAssignableTo(parsableType))
                        {
                            arguments[i] = parameterType.GetMethod(nameof(IParsable<>.Parse), [typeof(string), typeof(IFormatProvider)])!.Invoke(null, [stringArgument, null]);
                        }
                    }
                }
            }
        }
        
        UE_LOG(LogZSharpScript, $"Run cheat {string.Join(" ", arguments.Select(a => a?.ToString()).Prepend(command.Name))}.");
        return command.Method.Invoke(command.Cheater, arguments);
    }

    private void AuthorityCheat(string? command, params IEnumerable<object?> args)
    {
        if (UZGameplayStatics.GetPrimaryPlayerController() is not { } pc)
        {
            return;
        }

        if (pc.HasAuthority())
        {
            Cheat(command, args);
            return;
        }

        if (pc.GetComponent<UZrpgCheatComponent>() is not { } cheatComp)
        {
            return;
        }
        
        cheatComp.ServerCheat(command, [..args.Select(a => a?.ToString())]);
    }

    private readonly Dictionary<CheaterRegistration, ICheater> _cheaters = [];
    private readonly Dictionary<string, PriorityQueue<Command, int32>> _commands = [];
    private readonly Dictionary<string, PriorityQueue<Variable, int32>> _variables = [];

    private uint64 _cheaterHandle;
    
    #region ICheatEngine Implementations
    
    public object? Cheat(string? command, params IEnumerable<object?> args)
    {
        if (string.IsNullOrWhiteSpace(command))
        {
            return null;
        }

        string key = command.ToLower();
        if (!_commands.TryGetValue(key, out var queue))
        {
            goto CommandNotFound;
        }

        while (queue.TryPeek(out var cmd, out _))
        {
            if (cmd.Cheater is { } requiredCheater && !_cheaters.ContainsValue(requiredCheater))
            {
                queue.Dequeue();
                continue;
            }

            return Cheat(cmd, args);
        }

        CommandNotFound:
        {
            UE_WARNING(LogZSharpScript, $"Cheat command {command} not found!");
            return null;
        }
    }

    public CheaterRegistration RegisterCheater(ICheater? cheater)
    {
        if (cheater is null)
        {
            return default;
        }
        
        foreach (var (r, c) in _cheaters)
        {
            if (c == cheater)
            {
                return r;
            }
        }

        CheaterRegistration reg = new(this, ++_cheaterHandle);
        _cheaters[reg] = cheater;
        
        LootCheater(cheater.GetType(), cheater);

        return reg;
    }

    public void UnregisterCheater(CheaterRegistration registration)
    {
        _cheaters.Remove(registration);
    }

    public bool GetEnvironmentVariable(string? name, [NotNullWhen(true)] out object? value)
    {
        throw new NotImplementedException();
    }

    public bool SetEnvironmentVariable(string? name, object? value, int32 priority = -1, string? reason = null)
    {
        throw new NotImplementedException();
    }
    
    #endregion
    
}


