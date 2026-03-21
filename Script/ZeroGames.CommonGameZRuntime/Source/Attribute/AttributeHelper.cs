// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.CommonGameZRuntime;

public static class AttributeHelper<TKey, TValue>
	where TValue : IAdditiveIdentity<TValue, TValue>, IAdditionOperators<TValue, TValue, TValue>
{
	public static TValue GetAttributeFromDictionary(TKey key, IReadOnlyDictionary<TKey, TValue> dest)
		=> dest.GetValueOrDefault(key, TValue.AdditiveIdentity);

	// Trick: IDictionary is not IReadOnlyDictionary...
	public static TValue GetAttributeFromDictionary(TKey key, IDictionary<TKey, TValue> dest)
	{
		if (!dest.TryGetValue(key, out var value))
		{
			value = TValue.AdditiveIdentity;
		}

		return value;
	}
	
	public static bool SetAttributeToDictionary(TKey key, TValue value, IDictionary<TKey, TValue> dest, IEqualityComparer<TValue>? comparer, out TValue oldValue)
	{
		comparer ??= EqualityComparer<TValue>.Default;
		
		oldValue = GetAttributeFromDictionary(key, dest);
		if (comparer.Equals(oldValue, value))
		{
			return false;
		}
		
		if (!comparer.Equals(value, TValue.AdditiveIdentity))
		{
			dest[key] = value;
		}
		else
		{
			dest.Remove(key);
		}
		
		return true;
	}

	public static bool AddAttributeToDictionary(TKey key, TValue value, IDictionary<TKey, TValue> dest, IEqualityComparer<TValue>? comparer, out TValue oldValue)
	{
		comparer ??= EqualityComparer<TValue>.Default;
		
		oldValue = GetAttributeFromDictionary(key, dest);
		TValue newValue = oldValue + value;
		if (comparer.Equals(oldValue, newValue))
		{
			return false;
		}
		
		if (!comparer.Equals(newValue, TValue.AdditiveIdentity))
		{
			dest[key] = newValue;
		}
		else
		{
			dest.Remove(key);
		}
		
		return true;
	}
	
	public static bool AppendAttributesToDictionary(IEnumerable<KeyValuePair<TKey, TValue>>? src, IDictionary<TKey, TValue> dest, IEqualityComparer<TValue>? comparer)
	{
		if (src is null)
		{
			return false;
		}
		
		bool changed = false;
		foreach (var (key, value) in src)
		{
			changed |= AddAttributeToDictionary(key, value, dest, comparer, out _);
		}
		
		return changed;
	}
	
	public static bool AppendAttributesToDictionary(ReadOnlySpan<KeyValuePair<TKey, TValue>> src, IDictionary<TKey, TValue> dest, IEqualityComparer<TValue>? comparer)
	{
		bool changed = false;
		foreach (var (key, value) in src)
		{
			changed |= AddAttributeToDictionary(key, value, dest, comparer, out _);
		}
		
		return changed;
	}
}


