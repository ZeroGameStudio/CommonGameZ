// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

[UnrealFieldPath("/Script/CommonGameZRuntime.EZScriptClientGameInitializationState")]
[UEnum, BlueprintType]
public enum EZScriptClientGameInitializationState : uint8
{
	WaitingForController,
	WaitingForReplication,
	WaitingForGameplay,
	Initialized,
}

[UClass, NotBlueprintable]
public partial class UZScriptClientGameControllerSubsystem : UZClientGameControllerSubsystemBase
{

	public static UZScriptClientGameControllerSubsystem Instance
		=> UZGameplayStatics.GetPrimaryGameWorld()?.GetSubsystem<UZScriptClientGameControllerSubsystem>() ?? throw new InvalidOperationException();

	public EZScriptClientGameInitializationState InitializationState
	{
		[UFunction, BlueprintPure]
		get => InitializationStatePrivate;
	}

	public ZeroTask WhenInitialized
	{
		get
		{
			if (!__IsValid)
			{
				return ZeroTask.FromExpired(Lifetime);
			}

			if (_whenInitializedTcs is not null)
			{
				return _memoizedWhenInitialized!.Value;
			}

			if (InitializationStatePrivate is EZScriptClientGameInitializationState.Initialized)
			{
				return ZeroTask.CompletedTask;
			}
			
			_whenInitializedTcs = ZeroTaskCompletionSource.Create();
			_memoizedWhenInitialized = _whenInitializedTcs.Value.Task.Memoize();
			return _memoizedWhenInitialized.Value;
		}
	}

	protected override void ReceivedController_Implementation()
	{
		base.ReceivedController_Implementation();
		
		WaitUntilInitializedAsync().Forget();
		MonitorInitializationStateAsync().Forget();
	}

	private async ZeroTaskVoid MonitorInitializationStateAsync()
	{
		const float FIRST_DELAY_SECONDS = 30;
		const float INTERVAL_SECONDS = 5;
		
		await ZeroTask.Delay(FIRST_DELAY_SECONDS, Lifetime);

		if (InitializationStatePrivate is EZScriptClientGameInitializationState.Initialized)
		{
			return;
		}
		
		float elapsedSeconds = FIRST_DELAY_SECONDS;
		await foreach (var _ in ZeroStream.Interval(INTERVAL_SECONDS, Lifetime))
		{
			EZScriptClientGameInitializationState state = InitializationStatePrivate;
			if (state is EZScriptClientGameInitializationState.Initialized)
			{
				break;
			}
			
			elapsedSeconds += INTERVAL_SECONDS;
			UE_WARNING(LogCommonGameZRuntimeScript, $"Client game not initialized for {elapsedSeconds} seconds. Current state: {state}, waiting for: {_debugWaitingItem}");
		}
	}

	private async ZeroTaskVoid WaitUntilInitializedAsync()
	{
		const float INTERVAL_SECONDS = 0.1f;
		
		assert(InitializationStatePrivate is EZScriptClientGameInitializationState.WaitingForController);
		InitializationStatePrivate = EZScriptClientGameInitializationState.WaitingForReplication;

		while (!IsReplicationReady())
		{
			await ZeroTask.Delay(INTERVAL_SECONDS, Lifetime);
		}

		InitializationStatePrivate = EZScriptClientGameInitializationState.WaitingForGameplay;
		
		while (!IsGameplayReady())
		{
			await ZeroTask.Delay(INTERVAL_SECONDS, Lifetime);
		}

		InitializationStatePrivate = EZScriptClientGameInitializationState.Initialized;
		_debugWaitingItem = string.Empty;
		_whenInitializedTcs?.SetResult();
	}

	private bool IsReplicationReady()
	{
		if (UGameplayStatics.GetGameState(this) is null)
		{
			_debugWaitingItem = "Game State";
			return false;
		}

		if (UGameplayStatics.GetPlayerController(this, 0) is not {} pc)
		{
			_debugWaitingItem = "Player Controller";
			return false;
		}

		if (pc.PlayerState is null || pc.K2_GetPawn() is null)
		{
			_debugWaitingItem = "Player State";
			return false;
		}

		if (pc.K2_GetPawn() is null)
		{
			_debugWaitingItem = "Player Pawn";
			return false;
		}

		return true;
	}

	private bool IsGameplayReady()
	{
		return true;
	}

	[UProperty, Transient]
	private partial EZScriptClientGameInitializationState InitializationStatePrivate { get; set; }

	private string _debugWaitingItem = string.Empty;

	private ZeroTaskCompletionSource? _whenInitializedTcs;
	private ZeroTask? _memoizedWhenInitialized;

}




