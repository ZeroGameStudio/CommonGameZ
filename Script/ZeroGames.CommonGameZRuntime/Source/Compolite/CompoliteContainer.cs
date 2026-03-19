// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public class CompoliteContainer<TSchema> : ICompoliteLifecycle<TSchema> where TSchema : class
{
	
	public CompoliteContainer(TSchema owner)
	{
		this.To<ICompoliteLifecycle<TSchema>>().Initialize(owner);
	}
	
	public void RegisterCompolite(ICompolite<TSchema> compolite)
	{
		this.GuardLifecycleState(ECompoliteLifecycleState.Initialized);

		try
		{
			compolite.Initialize(_owner);
			// 按照契约，这一步过后组件必须是 Initialized，不能自己主动激活。
			if (compolite.LifecycleState is not ECompoliteLifecycleState.Initialized)
			{
				throw new InvalidOperationException();
			}
		}
		catch (Exception)
		{
			assert(false);
			UE_ERROR(LogZSharpScript, $"Failed to initialize compolite: {compolite.GetType().Name}.");
			return;
		}
		
		_compolites.Add(compolite);

		if (LifecycleState is ECompoliteLifecycleState.Activated)
		{
			GuardedActivateCompolite(compolite);
		}
	}

	public void UnregisterCompolite(ICompolite<TSchema> compolite)
	{
		if (compolite.Owner != _owner)
		{
			throw new ArgumentOutOfRangeException(nameof(compolite));
		}

		int32 i = _compolites.IndexOf(compolite);
		if (i < 0)
		{
			return;
		}

		if (compolite.LifecycleState is ECompoliteLifecycleState.Activated)
		{
			GuardedDeactivateCompolite(compolite);
		}
		
		_compolites.RemoveAt(i);
	}

	public T? GetCompolite<T>() where T : class => _compolites.OfType<T>().FirstOrDefault();

	public IEnumerable<ICompolite<TSchema>> Compolites => _compolites;

	private void GuardedActivateCompolite(ICompolite<TSchema> compolite)
	{
		if (compolite.LifecycleState is not ECompoliteLifecycleState.Initialized)
		{
			goto Error;
		}

		compolite.Activate();
		
		if (compolite.LifecycleState is not ECompoliteLifecycleState.Activated)
		{
			goto Error;
		}
		
		return;
		
		Error:
		{
			UE_ERROR(LogZSharpScript, $"Failed to activate compolite: {compolite.GetType().Name}.");
		}
	}
	
	private void GuardedDeactivateCompolite(ICompolite<TSchema> compolite)
	{
		if (compolite.LifecycleState is not ECompoliteLifecycleState.Activated)
		{
			goto Error;
		}

		compolite.Deactivate();
		
		if (compolite.LifecycleState is not ECompoliteLifecycleState.Deactivated)
		{
			goto Error;
		}

		return;
		
		Error:
		{
			UE_ERROR(LogZSharpScript, $"Failed to deactivate compolite: {compolite.GetType().Name}.");
		}
	}
	
	private TSchema _owner = null!;
	private readonly List<ICompolite<TSchema>> _compolites = [];
	
	#region ICompoliteLifecycle<TOwner> Implementations
	
	void ICompoliteLifecycle<TSchema>.Initialize(TSchema owner)
	{
		if (owner is not ICompoliteOwner<TSchema>)
		{
			throw new ArgumentOutOfRangeException(nameof(owner));
		}
		
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.NotInitialized);
		
		_owner = owner;
		LifecycleState = ECompoliteLifecycleState.Initialized;
	}
	
	public void Activate()
	{
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.Initialized);
		
		LifecycleState = ECompoliteLifecycleState.Activated;
		foreach (var compolite in _compolites)
		{
			GuardedActivateCompolite(compolite);
		}
	}

	public void Deactivate()
	{
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.Activated);

		for (int32 i = _compolites.Count - 1; i >= 0; --i)
		{
			var compolite = _compolites[i];
			GuardedDeactivateCompolite(compolite);
			_compolites.RemoveAt(i);
		}
		LifecycleState = ECompoliteLifecycleState.Deactivated;
	}

	public ECompoliteLifecycleState LifecycleState { get; private set; }

	#endregion
	
}


