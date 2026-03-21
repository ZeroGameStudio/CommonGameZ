// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.CommonGameZRuntime;

public sealed class AttributeSet<TKey, TValue> : AttributeSetBase<TKey, TValue>, IAttributeSet<TKey, TValue>
	where TKey : struct
	where TValue : struct, IAdditiveIdentity<TValue, TValue>, IAdditionOperators<TValue, TValue, TValue>
{
	
	public AttributeSet() : this(default(IEnumerable<KeyValuePair<TKey, TValue>>)){}
	public AttributeSet(IEnumerable<KeyValuePair<TKey, TValue>>? source = null, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
		: base(source, keyComparer, valueComparer){}
	public AttributeSet(ReadOnlySpan<KeyValuePair<TKey, TValue>> source, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
		: base(source, keyComparer, valueComparer){}
	
	#region IAttributeSet<TKey, TValue, TToken>

	public void SetAttribute(TKey key, TValue value)
	{
		if (AttributeHelper<TKey, TValue>.SetAttributeToDictionary(key, value, _storage, _comparer, out var oldValue))
		{
			NotifyAttributeValueChanged(key, oldValue, value);
		}
	}

	public void AddAttribute(TKey key, TValue value)
	{
		if (AttributeHelper<TKey, TValue>.AddAttributeToDictionary(key, value, _storage, _comparer, out var oldValue))
		{
			NotifyAttributeValueChanged(key, oldValue, value);
		}
	}

	public void AppendAttributes(IEnumerable<KeyValuePair<TKey, TValue>> attributes)
	{
		using BatchModificationScope scope = new(this);

		AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(attributes, _storage, _comparer);
	}

	public void AppendAttributes(ReadOnlySpan<KeyValuePair<TKey, TValue>> attributes)
	{
		using BatchModificationScope scope = new(this);
		
		AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(attributes, _storage, _comparer);
	}

	public void Clear()
	{
		using BatchModificationScope scope = new(this);
		
		_storage.Clear();
	}

	public void Modify(Action action)
	{
		using BatchModificationScope scope = new(this);

		action();
	}

	public void Modify(Action<IAttributeSet<TKey, TValue>> action)
	{
		using BatchModificationScope scope = new(this);

		action(this);
	}
	
	#endregion
	
}


