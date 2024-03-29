#pragma once

#include "ModularPlayerController.h"
#include "CommonPlayerController.generated.h"

UCLASS()
class COMMONGAME_API ACommonPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ACommonPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void ReceivedPlayer() override;
};

