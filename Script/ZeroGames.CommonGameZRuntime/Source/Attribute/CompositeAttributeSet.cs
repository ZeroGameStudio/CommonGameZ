// Copyright Zero Games. All Rights Reserved.

using System.Numerics;

namespace ZeroGames.CommonGameZRuntime;

public sealed class CompositeAttributeSet<TKey, TValue> : AttributeSetBase<TKey, TValue>, ICompositeAttributeSet<TKey, TValue>
	where TKey : struct
	where TValue : struct, IAdditiveIdentity<TValue, TValue>, IAdditionOperators<TValue, TValue, TValue>
{
	
	public CompositeAttributeSet() : this(default(IEnumerable<KeyValuePair<TKey, TValue>>)){}
	public CompositeAttributeSet(IEnumerable<KeyValuePair<TKey, TValue>>? source = null, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
		: base(source, keyComparer, valueComparer){}
	public CompositeAttributeSet(ReadOnlySpan<KeyValuePair<TKey, TValue>> source, IEqualityComparer<TKey>? keyComparer = null, IEqualityComparer<TValue>? valueComparer = null)
		: base(source, keyComparer, valueComparer){}
	
	private void InvalidateCache()
	{
		using BatchModificationScope scope = new(this);
		
		_storage.Clear();
		if (_components is null)
		{
			return;
		}
		
		foreach (var (component, _) in _components)
		{
			AttributeHelper<TKey, TValue>.AppendAttributesToDictionary(component, _storage, _comparer);
		}
	}

	private Dictionary<IReadOnlyAttributeSet<TKey, TValue>, EventRegistration>? _components;
	
	#region ICompositeAttributeSet Implementations

	public void RegisterComponent(IReadOnlyAttributeSet<TKey, TValue> component)
	{
		// TODO: Check circle.
		if (_components?.ContainsKey(component) is true)
		{
			return;
		}

		EventRegistration e = component.OnStateChanged.Add(_ => InvalidateCache());
		_components ??= new() { [component] = e };
		InvalidateCache();
	}

	public void UnregisterComponent(IReadOnlyAttributeSet<TKey, TValue> component)
	{
		if (_components?.Remove(component, out var e) is not true)
		{
			return;
		}
		
		e.Dispose();
		InvalidateCache();
	}
	
	#endregion
	
}


