#pragma once

#include "Engine/GameInstance.h"
#include "BattleGameInstance.generated.h"

UCLASS()
class UBattleGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UBattleGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;
	virtual void Shutdown() override;
	
};
