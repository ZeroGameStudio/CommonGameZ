// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public class GameObjectBase : IGameObject, IGameObjectLifecycle
	, ICompoliteOwner
	, IServiceCache
{
	protected GameObjectBase()
	{
		_les = LifetimeExpirationSource.Create();
		ReactiveLifetime = _les.ReactiveLifetime;

		_compoliteOwnerCore = new(this);
		_serviceCacheCore = new();
	}

	protected virtual void Initialized(){}
	protected virtual void BegunPlay(){}
	protected virtual void EndingPlay(){}

	private LifetimeExpirationSource _les;
	private CompoliteOwnerCore _compoliteOwnerCore;
	private ServiceCacheCore _serviceCacheCore;

	#region IGameObject Implementations

	public Lifetime Lifetime => ReactiveLifetime;
	public ReactiveLifetime ReactiveLifetime { get; }
	public EGameObjectLifecycleStage LifecycleStage { get; private set; }

	#endregion

	#region IGameObjectLifecycle Implementations

	void IGameObjectLifecycle.Initialize()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Allocated))
		{
			return;
		}

		LifecycleStage = EGameObjectLifecycleStage.Initialized;
		
		_compoliteOwnerCore.Initialize();
		Initialized();
	}

	void IGameObjectLifecycle.BeginPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Initialized))
		{
			return;
		}

		LifecycleStage = EGameObjectLifecycleStage.Playing;

		_compoliteOwnerCore.BeginPlay();
		BegunPlay();
	}

	void IGameObjectLifecycle.EndPlay()
	{
		if (!ensureAlways(LifecycleStage is EGameObjectLifecycleStage.Playing))
		{
			return;
		}

		EndingPlay();

		_compoliteOwnerCore.EndPlay();
		LifecycleStage = EGameObjectLifecycleStage.Dead;

		_les.SetExpired();
	}

	#endregion

	#region ICompoliteOwner Implementations

	public bool RegisterCompolite(Type? type, ICompoliteFactory? factory, [NotNullWhen(true)] out ICompolite? compolite)
		=> _compoliteOwnerCore.RegisterCompolite(type, factory, out compolite);

	public bool UnregisterCompolite(ICompolite compolite)
		=> _compoliteOwnerCore.UnregisterCompolite(compolite);

	public bool GetCompolite(Type type, [NotNullWhen(true)] out ICompolite? compolite)
		=> _compoliteOwnerCore.GetCompolite(type, out compolite);

	public IEnumerable<ICompolite> Compolites => _compoliteOwnerCore.Compolites;

	#endregion

	#region IServiceCache Implementations

	void IServiceCache.CacheService<T>(ServiceId serviceId, IService<T>? service) where T : class
	{
		_serviceCacheCore.TrySetService(serviceId, service);
	}

	#endregion
	
}


