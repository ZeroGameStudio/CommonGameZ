// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics;
using System.Runtime.CompilerServices;
using ZeroGames.ZSharp.UnrealEngine.GameplayTags;

namespace ZeroGames.CommonGameZRuntime;

public static class ActorComponentExtensions
{
    extension(UActorComponent @this)
    {
        public T? GetService<T>(ServiceId requiredId) where T : class, IService<T>
            => @this.GetOwner() is { } owner ? owner.GetService<T>(requiredId) : ServiceHelper.GetService<T, UActorComponent>(@this, requiredId);

        [Conditional("DEBUG")]
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void GuardAuthority()
        {
            assert(@this.GetOwnerChecked().HasAuthority());
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
        
        public void AddGameplayTag(GameplayTag tag)
        {
            FGameplayTagContainer tags = @this.EnsureAssetUserData<UZAssetUserData_GameplayTag>().Tags;
            UBlueprintGameplayTagLibrary.AddGameplayTag(ref tags, tag);
        }

        public void RemoveGameplayTag(GameplayTag tag)
        {
            if (@this.GetAssetUserData<UZAssetUserData_GameplayTag>() is not { } userdata)
            {
                return;
            }

            FGameplayTagContainer tags = userdata.Tags;
            UBlueprintGameplayTagLibrary.RemoveGameplayTag(ref tags, tag);
        }

        public bool HasGameplayTag(GameplayTag tag)
        {
            if (@this.GetAssetUserData<UZAssetUserData_GameplayTag>() is not { } userdata)
            {
                return false;
            }

            return UBlueprintGameplayTagLibrary.HasTag(userdata.Tags, tag, false);
        }

        public bool HasGameplayTagExact(GameplayTag tag)
        {
            if (@this.GetAssetUserData<UZAssetUserData_GameplayTag>() is not { } userdata)
            {
                return false;
            }

            return UBlueprintGameplayTagLibrary.HasTag(userdata.Tags, tag, true);
        }
        
        public bool IsServer => @this.GetNetMode() is not ENetMode.NM_Client;
        public bool IsOnlineServer => @this.GetNetMode() is not (ENetMode.NM_Client or ENetMode.NM_Standalone);
        public bool IsClient => @this.GetNetMode() is not ENetMode.NM_DedicatedServer;
    }
}


