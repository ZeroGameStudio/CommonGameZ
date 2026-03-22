// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.CommonGameZRuntime;

public struct ServiceCacheCore
{

    public bool TryGetService<T>(ServiceId serviceId, out T? service) where T : class, IService<T>
    {
        if (_cache is null)
        {
            service = null;
            return false;
        }
        
        Type serviceType = typeof(T);
        if (!_cache.TryGetValue(serviceType, out var cache))
        {
            service = null;
            return false;
        }

        if (!cache.TryGetValue(serviceId, out var s))
        {
            service = null;
            return false;
        }

        service = (T?)s;
        if (service?.IsServiceAvailable is false)
        {
            assert(false);
            
            cache.Remove(serviceId);
            
            service = null;
            return false;
        }
        
        return true;
    }

    public bool TrySetService<T>(ServiceId serviceId, IService<T>? service) where T : class, IService<T>
    {
        Type serviceType = typeof(T); // 注意：要缓存的是指定类型的服务，而不是服务对象的实际类型。
        if (!ServiceHelper.IsServiceTypeCacheable(serviceType))
        {
            return false;
        }

        if (service?.IsServiceAvailable is false)
        {
            return false;
        }

        if (service is not null && service.ServiceId != serviceId)
        {
            return false;
        }

        _cache ??= [];
        if (!_cache.TryGetValue(serviceType, out var cache))
        {
            cache = new() { [serviceId] = service };
            _cache[serviceType] = cache;
            return true;
        }

        if (cache.TryGetValue(serviceId, out var existing) && !ReferenceEquals(existing, service))
        {
            UE_WARNING(LogCommonGameZRuntimeScript, $"Service {{{serviceType.FullName}, {serviceId.Value}}} already cached a different instance!");
            return false;
        }

        cache[serviceId] = service;
        return true;
    }
    
    public void Invalidate()
    {
        _cache?.Clear();
    }

    public void Invalidate(Type serviceType)
    {
        _cache?.Remove(serviceType);
    }

    public void Invalidate(Type serviceType, ServiceId serviceId)
    {
        if (_cache?.TryGetValue(serviceType, out var cache) is true)
        {
            cache.Remove(serviceId);
        }
    }

    private Dictionary<Type, Dictionary<ServiceId, object?>>? _cache;

}


