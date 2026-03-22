// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.CommonGameZRuntime;

public static class ServiceHelper
{
	public static TService? GetService<TService, TTarget>(TTarget? target, ServiceId requiredId, Func<TTarget, ServiceId, TService?>? find = null)
		where TService : class, IService<TService>
		where TTarget : class
	{
		if (target is TService { IsServiceAvailable: true } selfAsService && selfAsService.ServiceId.Matches(requiredId))
		{
			return selfAsService;
		}

		if (target is IServiceOwner<TService> { IsServiceAvailable: true, Service: var staticService } && staticService.ServiceId.Matches(requiredId))
		{
			return staticService;
		}

		if (target is IServiceLocator selfAsLocator)
		{
			return selfAsLocator.GetService<TService>(requiredId);
		}
		
		if (target is null)
		{
			return null;
		}

		if (find?.Invoke(target, requiredId) is not { } service)
		{
			return null;
		}
		
		if (target is IServiceCache cache && IsServiceCacheable(service))
		{
			cache.CacheService(requiredId, service);
		}

		return service;
	}
	
	public static bool IsServiceCacheable<T>(T? service) where T : class, IService<T>
	{
		return service is { IsServiceAvailable: true } && IsServiceTypeCacheable(service.GetType());
	}
	
	public static bool IsServiceTypeCacheable(Type serviceType)
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

		if (attr.Lifetime >= EServiceLifetime.Implicit)
		{
			return false;
		}

		return true;
	}
}


