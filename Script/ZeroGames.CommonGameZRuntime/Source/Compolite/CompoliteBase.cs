// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public class CompoliteBase<TSchema> : ICompolite<TSchema> where TSchema : class
{
	protected virtual void Initialize(){}
	protected virtual void Activate(){}
	protected virtual void Deactivate(){}
	
	#region ICompolite<AActor> Implementations
	
	void ICompoliteLifecycle<TSchema>.Initialize(TSchema owner)
	{
		if (owner is not ICompoliteOwner<TSchema>)
		{
			throw new ArgumentOutOfRangeException(nameof(owner));
		}
		
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.NotInitialized);

		Owner = owner;
		LifecycleState = ECompoliteLifecycleState.Initialized;
		
		Initialize();
	}

	void ICompoliteLifecycle<TSchema>.Activate()
	{
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.Initialized);

		LifecycleState = ECompoliteLifecycleState.Activated;
		
		Activate();
	}

	void ICompoliteLifecycle<TSchema>.Deactivate()
	{
		this.GuardLifecycleStateExactly(ECompoliteLifecycleState.Activated);

		Deactivate();
		
		LifecycleState = ECompoliteLifecycleState.Deactivated;
	}

	public ECompoliteLifecycleState LifecycleState { get; private set; }
	
	[AllowNull]
	public TSchema Owner { get; private set; }
	
	#endregion
}


