// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IServiceCache
{
    void CacheService<T>(ServiceId serviceId, IService<T>? service) where T : class, IService<T>;
}


