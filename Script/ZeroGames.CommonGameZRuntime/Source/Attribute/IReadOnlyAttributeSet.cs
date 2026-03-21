// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IReadOnlyAttributeSet<TKey, TValue> : IEnumerable<KeyValuePair<TKey, TValue>>
{
	TValue GetAttribute(TKey key);
	
	IMemoizedObstreamEntry<TValue> GetAttributeStream(TKey key);
	
	IEventEntry<IReadOnlyAttributeSet<TKey, TValue>, TKey, TValue, TValue> OnAttributeValueChanged { get; }
	IEventEntry<IReadOnlyAttributeSet<TKey, TValue>> OnStateChanged { get; }
}


