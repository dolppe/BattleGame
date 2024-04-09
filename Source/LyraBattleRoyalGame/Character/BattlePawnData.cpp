#include "BattlePawnData.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraMode.h"
#include "LyraBattleRoyalGame/Input/BattleInputConfig.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePawnData)


UBattlePawnData::UBattlePawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	DefaultCameraMode = nullptr;
}
