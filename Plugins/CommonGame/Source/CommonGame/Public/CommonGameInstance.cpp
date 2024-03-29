#include "CommonGameInstance.h"

#include "CommonLocalPlayer.h"
#include "GameUIManagerSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonGameInstance)


UCommonGameInstance::UCommonGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

int32 UCommonGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	// GameInstance에서 관리하는 Player 컨테이너에 새로운 Player를 추가 (ReturnVal은 새로 추가된 Player의 Index)
	int ReturnVal = Super::AddLocalPlayer(NewPlayer, UserId);

	if (ReturnVal != INDEX_NONE)
	{
		// 메인 로컬 플레이어를 캐싱해두는 것.
		// 첫번째인 Index 0번인 로컬플레이어가 해당 클라이언트가 조종하는 로컬플레이어임
		if (!PrimaryPlayer.IsValid())
		{
			PrimaryPlayer = NewPlayer;
		}

		// GameUIManagerSubsystem을 통해 NotifyPlayerAdded를 호출해서 GameLayout을 추가함.
		GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerAdded(Cast<UCommonLocalPlayer>(NewPlayer));
	}

	return ReturnVal;
}

bool UCommonGameInstance::RemoveLocalPlayer(ULocalPlayer* ExistingPlayer)
{
	if (PrimaryPlayer == ExistingPlayer)
	{
		PrimaryPlayer.Reset();
	}

	GetSubsystem<UGameUIManagerSubsystem>()->NotifyPlayerDestroyed(Cast<UCommonLocalPlayer>(ExistingPlayer));
	
	return Super::RemoveLocalPlayer(ExistingPlayer);
}
