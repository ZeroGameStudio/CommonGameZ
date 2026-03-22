// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public abstract class CompoliteBase : ICompolite, ICompoliteLifecycle
{
	protected virtual void Initialized(){}
	protected virtual void BegunPlay(){}
	protected virtual void EndingPlay(){}
	
	#region ICompolite Implementations

	public ECompoliteLifecycleStage LifecycleStage { get; private set; }
	
	public abstract object? Owner { get; }
	
	#endregion
	
	#region ICompoliteLifecycle Implementations
	
	void ICompoliteLifecycle.Initialize()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Allocated);
		
		LifecycleStage = ECompoliteLifecycleStage.Initialized;
		
		Initialized();
	}

	void ICompoliteLifecycle.BeginPlay()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Initialized);

		LifecycleStage = ECompoliteLifecycleStage.Playing;
		
		BegunPlay();
	}

	void ICompoliteLifecycle.EndPlay()
	{
		this.GuardLifecycleStageExactly(ECompoliteLifecycleStage.Playing);

		EndingPlay();
		
		LifecycleStage = ECompoliteLifecycleStage.Dead;
	}
	
	#endregion
}


