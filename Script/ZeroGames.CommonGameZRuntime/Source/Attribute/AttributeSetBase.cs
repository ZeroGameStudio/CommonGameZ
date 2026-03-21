// Copyright Zero Games. All Rights Reserved.

using System.Collections;
using System.Numerics;
using System.Runtime.CompilerServices;

namespace ZeroGames.CommonGameZRuntime;

public abstract class AttributeSetBase<TKey, TValue> : IReadOnlyAttributeSet<TKey, TValue>
	where TKey : notnull
	where TValue : IAdditiveIdentity<TValue, TValue>, IAdditionOperators<TValue, TValue, TValue>
{
	
	protected readonly struct BatchModificationScope : IDisposable
	{
		public BatchModificationScope(AttributeSetBase<TKey, TValue> target)
		{
			_target = target;
			_snapshot = _target._storage.ToDictionary();
		}

		private readonly AttributeSetBase<TKey, TValue> _target;
		private readonly Dictionary<TKey, TValue> _snapshot;
		
		#region IDisposable Implementations
		
		public void Dispose()
		{
			bool changed = false;
			foreach (var (key, value) in _target._storage)
			{
				TValue oldValue = _snapshot.GetValueOrDefault(key, TValue.AdditiveIdentity);
				if (!_target._comparer.Equals(oldValue, value))
				{
					changed = true;
					_target.NotifyAttributeValueChanged(key, oldValue, value);
				}
			}
			
			foreach (var (key, value) in _snapshot)
			{
				if (!_target._storage.ContainsKey(key) && !_target._comparer.Equals(value, TValue.AdditiveIdentity))
				{
					changed = true;
					_target.NotifyAttributeValueChanged(key, value, TValue.AdditiveIdentity);
				}
			}

			if (changed)
			{
				_target.NotifyStateChanged();
			}
		}
		
		#endregion
	}

	protected AttributeSetBase(IEnumerable<KeyValuePair<TKey, TValue>>? source, IEqualityComparer<TKey>? keyComparer, IEqualityComparer<TValue>? valueComparer)
	{
		_storage = new(keyComparer);
		_comparer = valueComparer ?? EqualityComparer<TValue>.Default;
		
		AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(source, _storage, _comparer);
	}

	protected AttributeSetBase(ReadOnlySpan<KeyValuePair<TKey, TValue>> source, IEqualityComparer<TKey>? keyComparer, IEqualityComparer<TValue>? valueComparer)
	{
		_storage = new(keyComparer);
		_comparer = valueComparer ?? EqualityComparer<TValue>.Default;
		
		AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(source, _storage, _comparer);
	}

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	protected void NotifyAttributeValueChanged(TKey key, TValue oldValue, TValue newValue)
	{
		if (_streams?.TryGetValue(key, out var stream) is true)
		{
			stream.Value = newValue;
		}
		
		_onAttributeValueChanged?.Invoke(this, key, oldValue, newValue);
	}
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	protected void NotifyStateChanged()
		=> _onStateChanged?.Invoke(this);
	
	protected readonly Dictionary<TKey, TValue> _storage;
	protected readonly IEqualityComparer<TValue> _comparer;
	
	private Dictionary<TKey, MemoizedObstream<TValue>>? _streams;

	private Event<IReadOnlyAttributeSet<TKey, TValue>, TKey, TValue, TValue>? _onAttributeValueChanged;
	private Event<IReadOnlyAttributeSet<TKey, TValue>>? _onStateChanged;
	
	#region IReadOnlyAttributeSet<TKey, TValue> Implementations

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator() => _storage.GetEnumerator();
	
	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

	[MethodImpl(MethodImplOptions.AggressiveInlining)]
	public TValue GetAttribute(TKey key) => _storage.GetValueOrDefault(key, TValue.AdditiveIdentity);

	public IMemoizedObstreamEntry<TValue> GetAttributeStream(TKey key)
	{
		if (_streams?.TryGetValue(key, out var result) is not true)
		{
			result = new(GetAttribute(key));
			_streams ??= [];
			_streams[key] = result;
		}

		return result;
	}

	public IEventEntry<IReadOnlyAttributeSet<TKey, TValue>, TKey, TValue, TValue> OnAttributeValueChanged
		=> _onAttributeValueChanged ??= new();

	public IEventEntry<IReadOnlyAttributeSet<TKey, TValue>> OnStateChanged
		=> _onStateChanged ??= new();

	#endregion
	
}


