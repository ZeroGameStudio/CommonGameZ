// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public struct CompoliteOwnerCore : ICompoliteOwner, ICompoliteLifecycle
{

	public CompoliteOwnerCore(object? owner)
	{
		_owner = owner;
	}
	
	private void RouteBeginPlay(ICompolite compolite)
	{
		if (compolite.LifecycleStage is not ECompoliteLifecycleStage.Initialized)
		{
			goto Error;
		}

		compolite.To<ICompoliteLifecycle>().BeginPlay();
		
		if (compolite.LifecycleStage is not ECompoliteLifecycleStage.Playing)
		{
			goto Error;
		}
		
		return;
		
		Error:
		{
			UE_ERROR(LogCommonGameZRuntimeScript, $"Failed to activate compolite: {compolite.GetType().Name}.");
		}
	}
	
	private void RouteEndPlay(ICompolite compolite)
	{
		if (compolite.LifecycleStage is not ECompoliteLifecycleStage.Playing)
		{
			goto Error;
		}

		compolite.To<ICompoliteLifecycle>().EndPlay();
		
		if (compolite.LifecycleStage is not ECompoliteLifecycleStage.Dead)
		{
			goto Error;
		}

		return;
		
		Error:
		{
			UE_ERROR(LogCommonGameZRuntimeScript, $"Failed to deactivate compolite: {compolite.GetType().Name}.");
		}
	}
	
	private readonly object? _owner;
	private List<ICompolite>? _compolites;
	
	#region ICompoliteOwner Implementations
	
	public bool RegisterCompolite(Type? type, ICompoliteFactory? factory, [NotNullWhen(true)] out ICompolite? compolite)
	{
		this.GuardLifecycleStage(ECompoliteLifecycleStage.Initialized);

		if (type is null)
		{
			compolite = null;
			return false;
		}

		factory ??= CompoliteFactory.Instance;
		if (!factory.TryAllocateCompolite(type, _owner, out compolite))
		{
			return false;
		}

		try
		{
			compolite.To<ICompoliteLifecycle>().Initialize();
			compolite.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Initialized);
		}
		catch (Exception)
		{
			assert(false);
			UE_ERROR(LogCommonGameZRuntimeScript, $"Failed to initialize compolite: {compolite.GetType().Name}.");
			return false;
		}

		_compolites ??= [];
		_compolites.Add(compolite);

		if (LifecycleStage is ECompoliteLifecycleStage.Playing)
		{
			RouteBeginPlay(compolite);
		}

		return true;
	}

	public bool UnregisterCompolite(ICompolite compolite)
	{
		if (compolite.Owner != _owner)
		{
			throw new ArgumentOutOfRangeException(nameof(compolite));
		}

		if (_compolites is null)
		{
			return false;
		}

		int32 i = _compolites.IndexOf(compolite);
		if (i < 0)
		{
			return false;
		}

		if (compolite.LifecycleStage is ECompoliteLifecycleStage.Playing)
		{
			RouteEndPlay(compolite);
		}
		
		_compolites.RemoveAt(i);
		return true;
	}

	public bool GetCompolite(Type type, [NotNullWhen(true)] out ICompolite? compolite)
	{
		compolite = null;
		if (_compolites is null)
		{
			return false;
		}
		
		foreach (var comp in _compolites)
		{
			if (comp.GetType() == type)
			{
				compolite = comp;
				break;
			}
		}

		return compolite is not null;
	}
	
	public IEnumerable<ICompolite> Compolites => _compolites ?? [];
	
	#endregion
	
	#region ICompoliteLifecycle Implementations
	
	public void Initialize()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Allocated);
		
		LifecycleStage = ECompoliteLifecycleStage.Initialized;
	}
	
	public void BeginPlay()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Initialized);
		
		LifecycleStage = ECompoliteLifecycleStage.Playing;
		if (_compolites is not null)
		{
			foreach (var compolite in _compolites)
			{
				RouteBeginPlay(compolite);
			}
		}
	}

	public void EndPlay()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Playing);

		if (_compolites is not null)
		{
			for (int32 i = _compolites.Count - 1; i >= 0; --i)
			{
				var compolite = _compolites[i];
				RouteEndPlay(compolite);
				_compolites.RemoveAt(i);
			}
		}
		
		LifecycleStage = ECompoliteLifecycleStage.Dead;
	}

	public ECompoliteLifecycleStage LifecycleStage { get; private set; }

	#endregion
	
}


