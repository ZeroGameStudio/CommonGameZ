// Copyright Zero Games. All Rights Reserved.

#include "Gameplay/Movement/ZActorMovementReplicationHandler.h"

bool IZActorMovementReplicationHandler::FindHandler(AActor* self, TScriptInterface<IZActorMovementReplicationHandler>& handler)
{
	if (self->Implements<UZActorMovementReplicationHandler>())
	{
		handler = self;
		return true;
	}
	else if (UActorComponent* handlerComp = self->FindComponentByInterface(UZActorMovementReplicationHandler::StaticClass()))
	{
		handler = handlerComp;
		return true;
	}

	return false;
}


