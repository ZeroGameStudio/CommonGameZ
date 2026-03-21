// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IAttributeSet<TKey, TValue> : IReadOnlyAttributeSet<TKey, TValue>
{
	void SetAttribute(TKey key, TValue value);
	void AddAttribute(TKey key, TValue value);
	void AppendAttributes(IEnumerable<KeyValuePair<TKey, TValue>> attributes);
	void AppendAttributes(ReadOnlySpan<KeyValuePair<TKey, TValue>> attributes);
	void Clear();
	
	void Modify(Action action);
	void Modify(Action<IAttributeSet<TKey, TValue>> action);
}


