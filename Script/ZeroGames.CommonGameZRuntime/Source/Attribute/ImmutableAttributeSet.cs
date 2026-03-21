// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Collections.Immutable;
using System.Numerics;

namespace ZeroGames.CommonGameZRuntime;

public sealed class ImmutableAttributeSet<TKey, TValue> : IImmutableAttributeSet<TKey, TValue>
	where TKey : struct
	where TValue : struct, IAdditiveIdentity<TValue, TValue>, IAdditionOperators<TValue, TValue, TValue>
{
	public ImmutableAttributeSet() : this(default(IEnumerable<KeyValuePair<TKey, TValue>>)){}
	public ImmutableAttributeSet(IEnumerable<KeyValuePair<TKey, TValue>>? source = null, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
	{
		if (source is not null)
		{
			Dictionary<TKey, TValue> temp = new(keyComparer);
			AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(source, temp, valueComparer);
			_storage = temp.ToImmutableDictionary(keyComparer, valueComparer);
		}
		else
		{
			_storage = ImmutableDictionary<TKey, TValue>.Empty;
		}
	}

	public ImmutableAttributeSet(ReadOnlySpan<KeyValuePair<TKey, TValue>> source, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
	{
		if (!source.IsEmpty)
		{
			Dictionary<TKey, TValue> temp = new(keyComparer);
			AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(source, temp, valueComparer);
			_storage = temp.ToImmutableDictionary(keyComparer, valueComparer);
		}
		else
		{
			_storage = ImmutableDictionary<TKey, TValue>.Empty;
		}
	}

	private readonly ImmutableDictionary<TKey, TValue> _storage;
	
	#region IImmutableAttributeSet<TKey, TValue>
	
	public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator() => _storage.GetEnumerator();
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	public TValue GetAttribute(TKey key) => _storage.GetValueOrDefault(key, TValue.AdditiveIdentity);

	public IMemoizedObstreamEntry<TValue> GetAttributeStream(TKey key)
		=> NeverMemoizedObstream<TValue>.Instance;

	public IEventEntry<IReadOnlyAttributeSet<TKey, TValue>, TKey, TValue, TValue> OnAttributeValueChanged
		=> NeverEvent<IReadOnlyAttributeSet<TKey, TValue>, TKey, TValue, TValue>.Instance;

	public IEventEntry<IReadOnlyAttributeSet<TKey, TValue>> OnStateChanged
		=> NeverEvent<IReadOnlyAttributeSet<TKey, TValue>>.Instance;

	#endregion
}


