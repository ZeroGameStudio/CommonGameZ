// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface ICompositeAttributeSet<TKey, TValue> : IReadOnlyAttributeSet<TKey, TValue>
{
	void RegisterComponent(IReadOnlyAttributeSet<TKey, TValue> component);
	void UnregisterComponent(IReadOnlyAttributeSet<TKey, TValue> component);
}


