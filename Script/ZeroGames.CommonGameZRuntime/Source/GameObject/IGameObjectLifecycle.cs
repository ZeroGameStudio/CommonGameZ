// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IGameObjectLifecycle : IGameObjectLifecycleSource
{
	void Initialize();
	void BeginPlay();
	void EndPlay();
}


