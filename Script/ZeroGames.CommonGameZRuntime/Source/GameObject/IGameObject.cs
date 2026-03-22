// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface IReadOnlyGameObject : IGameObjectLifecycleSource, IReactiveLifetimeSource
{
	
}

public interface IGameObject : IReadOnlyGameObject
{
	
}

public static class GameObjectExtensions
{
	extension(IReadOnlyGameObject @this)
	{
		public T? GetService<T>(ServiceId requiredId) where T : class, IService<T>
            => ServiceHelper.GetService(@this, requiredId, static (go, requiredId) =>
            {
                T? service = null;
                if (go is ICompoliteOwner compoliteOwner)
                {
                    foreach (var compolite in compoliteOwner.Compolites.OfType<T>())
                    {
                        if (compolite is { IsServiceAvailable: true } && compolite.ServiceId.Matches(requiredId))
                        {
                            service = compolite;
                            break;
                        }
                    }
                }
                
                return service;
            });
	}
}


