// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface ICompoliteLifecycleSource
{
	ECompoliteLifecycleStage LifecycleStage { get; }
}

public static class CompoliteLifecycleSourceExtensions
{
	extension(ICompoliteLifecycleSource @this)
	{
		public void GuardLifecycleStage(ECompoliteLifecycleStage minStage)
		{
			if (@this.LifecycleStage < minStage)
			{
				throw new InvalidOperationException();
			}
		}

		public void GuardLifecycleStageExactly(ECompoliteLifecycleStage requiredStage)
		{
			if (@this.LifecycleStage != requiredStage)
			{
				throw new InvalidOperationException();
			}
		}
	}
}


