// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface ICompoliteLifecycle : ICompoliteLifecycleSource
{
	void Initialize();
	void BeginPlay();
	void EndPlay();
}


