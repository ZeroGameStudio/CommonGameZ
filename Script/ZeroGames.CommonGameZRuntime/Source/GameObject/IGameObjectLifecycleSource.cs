// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public enum EGameObjectLifecycleStage : uint8
{
	Allocated,
	Initialized,
	Playing,
	Dead,
}

public interface IGameObjectLifecycleSource
{
	EGameObjectLifecycleStage LifecycleStage { get; }
}


