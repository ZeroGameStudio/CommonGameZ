// Copyright Zero Games. All Rights Reserved.

#include "Engine/ZLocalPlayerBase.h"

void UZLocalPlayerBase::ReceivedPlayerController(APlayerController* newPC)
{
	Super::ReceivedPlayerController(newPC);

	OnPlayerControllerChanged.Broadcast(newPC);
}


