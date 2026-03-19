// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public enum EGameObjectLifecycleStage : uint8
{
	Initialized,
	Playing,
	Dead,
}

public interface IReadOnlyGameObject : IReactiveLifetimeSource
{
	EGameObjectLifecycleStage LifecycleStage { get; }
}

public interface IReadOnlyGameObject<out TContext> : IReadOnlyGameObject
{
	TContext Context { get; }
}

public interface IGameObject : IReadOnlyGameObject
{

}

public interface IGameObject<TContext> : IReadOnlyGameObject<TContext>
{
	
}


