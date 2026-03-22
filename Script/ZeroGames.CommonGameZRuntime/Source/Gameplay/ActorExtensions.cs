// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace ZeroGames.CommonGameZRuntime;

public static class ActorExtensions
{
    public static Func<AActor, ZeroTask> FallbackKillActorDelegate { get; set; } = static actor =>
    {
        actor.K2_DestroyActor();
        return ZeroTask.CompletedTask;
    };
    
    extension(AActor @this)
    {
        public T? GetService<T>(ServiceId requiredId) where T : class, IService<T>
            => ServiceHelper.GetService(@this, requiredId, static (actor, requiredId) =>
            {
                T? service = null;
                if (actor is ICompoliteOwner compoliteOwner)
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

                if (service is null)
                {
                    Type serviceType = typeof(T);
                    if (serviceType.IsSubclassOf(typeof(UActorComponent)))
                    {
                        if ((service = actor.GetComponentByClass(UClass.FromType(serviceType)) as T) is { IsServiceAvailable: false })
                        {
                            service = null;
                        }
                    }
                    else
                    {
                        foreach (var component in actor.K2_GetComponentsByClass(UActorComponent.StaticClass))
                        {
                            if (component is T { IsServiceAvailable: true } serviceComponent && serviceComponent.ServiceId.Matches(requiredId))
                            {
                                service = serviceComponent;
                                break;
                            }
                        }

                        #if ASSERTION_CHECK
                        
                        if (actor is not IServiceCache || !ServiceHelper.IsServiceCacheable(service))
                        {
                            UE_WARNING(LogCommonGameZRuntimeScript, $"Query actor service using slow path! Actor: {actor.GetName()}");
                        }
                        
                        #endif
                    }
                }

                return service;
            });
        
        [Conditional("DEBUG")]
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void GuardAuthority()
        {
            assert(@this.HasAuthority());
        }

        [Conditional("DEBUG")]
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void GuardClient()
        {
            assert(@this.IsClient);
        }
        
        [Conditional("DEBUG")]
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void GuardClientOnly()
        {
            @this.GuardClient();
            assert(!@this.bReplicates);
        }

        public ZeroTask KillAsync()
        {
            if (@this is IKillActorSource source)
            {
                return source.KillAsync();
            }
            else
            {
                return FallbackKillActorDelegate(@this);
            }
        }

        public void Kill() => KillAsync(@this).Forget();
        
        public bool IsServer => @this.GetNetMode() is not ENetMode.NM_Client;
        public bool IsOnlineServer => @this.GetNetMode() is not (ENetMode.NM_Client or ENetMode.NM_Standalone);
        public bool IsClient => @this.GetNetMode() is not ENetMode.NM_DedicatedServer;
    }
}


