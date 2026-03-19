// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public class GameObjectBase : IGameObject, IGameObjectLifecycle
{
	protected GameObjectBase()
	{
		_les = LifetimeExpirationSource.Create();
		ReactiveLifetime = _les.ReactiveLifetime;
	}

	protected virtual void BegunPlay(){}
	protected virtual void EndingPlay(){}

	private LifetimeExpirationSource _les;
	
	#region IGameObject Implementations
	
	public ReactiveLifetime ReactiveLifetime { get; }
	public EGameObjectLifecycleStage LifecycleStage { get; private set; }
	
	#endregion
	
	#region IGameObjectLifecycle Implementations
	
	void IGameObjectLifecycle.BeginPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Initialized))
		{
			return;
		}

		LifecycleStage = EGameObjectLifecycleStage.Playing;
		
		BegunPlay();
	}

	void IGameObjectLifecycle.EndPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Playing))
		{
			return;
		}
		
		EndingPlay();
		
		LifecycleStage = EGameObjectLifecycleStage.Dead;
		
		_les.SetExpired();
	}
	
	#endregion
}

public class GameObjectBase<TContext> : IGameObject<TContext>, IGameObjectLifecycle
{
	protected GameObjectBase(TContext context)
	{
		_les = LifetimeExpirationSource.Create();
		ReactiveLifetime = _les.ReactiveLifetime;
		Context = context;
	}

	protected virtual void BegunPlay(){}
	protected virtual void EndingPlay(){}

	private LifetimeExpirationSource _les;
	
	#region IGameObject Implementations

	public Lifetime Lifetime => ReactiveLifetime;
	public ReactiveLifetime ReactiveLifetime { get; }
	public EGameObjectLifecycleStage LifecycleStage { get; private set; }
	public TContext Context { get; }
	
	#endregion
	
	#region IGameObjectLifecycle Implementations
	
	void IGameObjectLifecycle.BeginPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Initialized))
		{
			return;
		}

		LifecycleStage = EGameObjectLifecycleStage.Playing;
		
		BegunPlay();
	}

	void IGameObjectLifecycle.EndPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Playing))
		{
			return;
		}
		
		EndingPlay();
		
		LifecycleStage = EGameObjectLifecycleStage.Dead;
		
		_les.SetExpired();
	}
	
	#endregion
}


