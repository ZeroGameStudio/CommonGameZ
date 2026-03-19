// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IServiceLocator
{
	T? GetService<T>(ServiceId requiredId) where T : class, IService<T>;
}


