// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public static class EngineSubsystemExtensions
{
	extension<T>(T @this) where T : UEngineSubsystem
	{
		public static T Instance => UEngine.Instance.GetSubsystem<T>() ?? throw new InvalidOperationException();
	}
}

public static class GameInstanceSubsystemExtensions
{
	extension<T>(T @this) where T : UGameInstanceSubsystem
	{
		public static T PrimaryInstance => UGameInstance.PrimaryInstance?.GetSubsystem<T>() ?? throw new InvalidOperationException();
	}
}

public static class WorldSubsystemExtensions
{
	extension<T>(T @this) where T : UWorldSubsystem
	{
		public static T PrimaryInstance => UZGameplayStatics.GetPrimaryGameWorld()?.GetSubsystem<T>() ?? throw new InvalidOperationException();
	}
}

public static class LocalPlayerSubsystemExtensions
{
	extension<T>(T @this) where T : ULocalPlayerSubsystem
	{
		public static T PrimaryInstance => UZGameplayStatics.GetPrimaryLocalPlayer()?.GetSubsystem<T>() ?? throw new InvalidOperationException();
	}
}


