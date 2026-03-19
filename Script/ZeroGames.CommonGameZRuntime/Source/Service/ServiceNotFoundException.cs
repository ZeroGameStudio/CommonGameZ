// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public class ServiceNotFoundException<T>(Exception? innerException = null)
	: Exception($"Service {typeof(T).Name} not found.", innerException)
	where T : class, IService<T>;


