// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public interface IReadOnlyBlackboard : IReadOnlyDictionary<string, object?>
{
	bool TryGetValue<T>(string key, [MaybeNullWhen(false)] out T value);
	
	bool TryGetValue<T>(BlackboardKey<T> key, [MaybeNullWhen(false)] out T value)
		=> TryGetValue(key.Key, out value);
}

public interface IBlackboard : IReadOnlyBlackboard
{
	void SetValue(string key, object? value);
	void ClearValue(string key);
	
	void SetValue<T>(BlackboardKey<T> key, T value)
		=> SetValue(key.Key, value);
}

public static class BlackboardExtensions
{
	extension(IReadOnlyBlackboard @this)
	{
		bool TryGetValue<T>(BlackboardKey<T> key, [MaybeNullWhen(false)] out T value)
			=> @this.TryGetValue(key.Key, out value);
	}
	
	extension(IBlackboard @this)
	{
		void SetValue<T>(BlackboardKey<T> key, T value)
			=> @this.SetValue(key, value);
	}
}


