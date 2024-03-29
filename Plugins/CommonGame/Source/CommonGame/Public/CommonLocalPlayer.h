#pragma once

#include "Engine/LocalPlayer.h"
#include "Delegates/Delegate.h"
#include "CommonLocalPlayer.generated.h"

UCLASS()
class COMMONGAME_API UCommonLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()
public:
	UCommonLocalPlayer();

	// Player Controller가 local player에 assign되었을 때 실행할 Delegate
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerControllerSetDelegate, UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	FPlayerControllerSetDelegate OnPlayerControllerSet;

	// Player State가 local player에 assign되었을 때 실행할 Delegate
	DECLARE_MULTICAST_DELEGATE_TwoParams(FPlayerStateSetDelegate, UCommonLocalPlayer* LocalPlayer, APlayerState* PlayerState)
	FPlayerStateSetDelegate OnPlayerStateSet;

	FDelegateHandle CallAndRegister_OnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate);

};
