// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public enum ECompoliteLifecycleState : uint8
{
	NotInitialized,
	Initialized,
	Activated,
	Deactivated,
}

public interface ICompoliteLifecycle<TSchema> where TSchema : class
{
	void Initialize(TSchema owner);
	void Activate();
	void Deactivate();

	ECompoliteLifecycleState LifecycleState { get; }
}

public static class CompoliteLifecycleExtensions
{
	extension<TSchema>(ICompoliteLifecycle<TSchema> @this) where TSchema : class
	{
		public void GuardLifecycleState(ECompoliteLifecycleState minState)
		{
			if (@this.LifecycleState < minState)
			{
				throw new InvalidOperationException();
			}
		}

		public void GuardLifecycleStateExactly(ECompoliteLifecycleState requiredState)
		{
			if (@this.LifecycleState != requiredState)
			{
				throw new InvalidOperationException();
			}
		}
	}
}


