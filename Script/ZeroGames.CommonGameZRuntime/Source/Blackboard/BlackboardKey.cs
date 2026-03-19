// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public readonly struct BlackboardKey<T>(string key)
{
	public string Key { get; } = key;
}


