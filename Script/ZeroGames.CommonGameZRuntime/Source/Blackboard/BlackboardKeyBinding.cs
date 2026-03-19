// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public readonly struct BlackboardKeyBinding<T>
{
	public BlackboardKeyBinding(string? keyBinding)
	{
		_keyBinding = keyBinding;
	}

	public BlackboardKeyBinding(BlackboardKey<T> keyBinding)
	{
		_keyBinding = keyBinding.Key;
	}

	public BlackboardKeyBinding(T? inlineValue)
	{
		_inlineValue = inlineValue;
	}

	public bool TryGetValue(IReadOnlyBlackboard blackboard, [MaybeNullWhen(false)] out T value)
	{
		if (_keyBinding is null)
		{
			value = _inlineValue;
			return value is not null;
		}
		else
		{
			return blackboard.TryGetValue(_keyBinding, out value);
		}
	}

	public T GetValueOrDefault(IReadOnlyBlackboard blackboard, T defaultValue)
	{
		if (TryGetValue(blackboard, out var value))
		{
			return value;
		}

		return defaultValue;
	}

	private readonly string? _keyBinding;
	private readonly T? _inlineValue;
}


