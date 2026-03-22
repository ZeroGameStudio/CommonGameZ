// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public class Blackboard : IBlackboard
{

	public void SetValue(string key, object? value, bool allowsPreserved)
	{
		if (!allowsPreserved && IsKeyPreserved?.Invoke(key) is true)
		{
			throw new InvalidOperationException();
		}
		
		object? existing = _storage.GetValueOrDefault(key);
		if (existing is not null && value is not null)
		{
			Type t1 = existing.GetType();
			Type t2 = value.GetType();
			if (!t1.IsAssignableTo(t2) && !t2.IsAssignableTo(t1))
			{
				UE_WARNING(LogCommonGameZRuntimeScript, $"[BehaviorTree.SetBlackboardValue] Unrelated types {t1.FullName} and {t2.FullName}.");
			}
		}
		
		_storage[key] = value;
	}

	public void SetValue<T>(BlackboardKey<T> key, T value, bool allowsPreserved)
		=> SetValue(key.Key, value, allowsPreserved);

	public void ClearValue(string key, bool allowsPreserved)
	{
		if (!allowsPreserved && IsKeyPreserved?.Invoke(key) is true)
		{
			throw new InvalidOperationException();
		}
		
		_storage.Remove(key);
	}

	public Func<string, bool>? IsKeyPreserved { get; init; }
	
	private readonly Dictionary<string, object?> _storage = [];
	
	#region IBlackboard Implementations

	public bool TryGetValue(string key, out object? value)
		=> _storage.TryGetValue(key, out value);

	public bool TryGetValue<T>(string key, [MaybeNullWhen(false)] out T value)
	{
		if (_storage.TryGetValue(key, out var v) && v is T vt)
		{
			value = vt;
			return true;
		}

		value = default;
		return false;
	}

	public void SetValue(string key, object? value)
		=> SetValue(key, value, false);

	public void ClearValue(string key)
		=> ClearValue(key, false);

	public bool ContainsKey(string key)
		=> _storage.ContainsKey(key);

	public IEnumerator<KeyValuePair<string, object?>> GetEnumerator() => _storage.GetEnumerator();
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	public object? this[string key] => _storage[key];

	public int32 Count => _storage.Count;

	public IEnumerable<string> Keys => _storage.Keys;
	public IEnumerable<object?> Values => _storage.Values;

	#endregion

}


