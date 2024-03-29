#pragma once

#include "CommonGameInstance.h"
#include "BattleGameInstance.generated.h"

UCLASS()
class UBattleGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()
public:
	UBattleGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Init() override;
	virtual void Shutdown() override;
	
};
