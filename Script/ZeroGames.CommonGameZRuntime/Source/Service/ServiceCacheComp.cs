// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.CommonGameZRuntime;

public class ServiceCacheComp
{

    public bool TryGetService<T>(ServiceId serviceId, out T? service) where T : class, IService<T>
    {
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
        if (!IsServiceTypeCacheable(serviceType))
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
        
        if (!_cache.TryGetValue(serviceType, out var cache))
        {
            cache = new() { [serviceId] = service };
            _cache[serviceType] = cache;
            return true;
        }

        if (cache.TryGetValue(serviceId, out var existing) && !ReferenceEquals(existing, service))
        {
            UE_WARNING(LogZSharpScript, $"Service {{{serviceType.FullName}, {serviceId.Value}}} already cached a different instance!");
            return false;
        }

        cache[serviceId] = service;
        return true;
    }
    
    public bool IsServiceTypeCacheable(Type serviceType)
    {
        if (!serviceType.IsClass || serviceType.IsInterface || serviceType.IsAbstract)
        {
            return false;
        }

        if (!serviceType.IsAssignableTo(typeof(IService<>).MakeGenericType(serviceType)))
        {
            return false;
        }

        if (serviceType.GetCustomAttribute<ServiceLifetimeAttribute>() is not { } attr)
        {
            return false;
        }

        if (attr.Lifetime is EServiceLifetime.Dynamic)
        {
            return false;
        }

        return true;
    }
    
    public void Invalidate()
    {
        _cache.Clear();
    }

    public void Invalidate(Type serviceType)
    {
        _cache.Remove(serviceType);
    }

    public void Invalidate(Type serviceType, ServiceId serviceId)
    {
        if (_cache.TryGetValue(serviceType, out var cache))
        {
            cache.Remove(serviceId);
        }
    }

    private readonly Dictionary<Type, Dictionary<ServiceId, object?>> _cache = [];

}


