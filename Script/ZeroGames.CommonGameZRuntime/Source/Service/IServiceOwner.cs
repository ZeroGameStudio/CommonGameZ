// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IServiceOwner<out T> where T : class, IService<T>
{
	bool IsServiceAvailable { get; }
	T Service { get; }
}


