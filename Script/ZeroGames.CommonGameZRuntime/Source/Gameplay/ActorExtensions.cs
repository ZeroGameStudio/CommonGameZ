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


