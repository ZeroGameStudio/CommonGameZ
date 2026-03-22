// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public enum ECompoliteLifecycleStage : uint8
{
	Allocated,
	Initialized,
	Playing,
	Dead,
}

public interface ICompolite : ICompoliteLifecycleSource
{
	object? Owner { get; }
}

public static class CompoliteExtensions
{
	extension(ICompolite @this)
	{
		public T? GetOwner<T>() where T : class => @this.Owner as T;
		public T GetOwnerChecked<T>() where T : class => @this.Owner as T ?? throw new InvalidOperationException();
	}
}


