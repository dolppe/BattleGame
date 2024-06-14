#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

// 싱글톤이며, Native Gameplay Tag들을 포함함.
struct FBattleGameplayTags
{

public:
	static const FBattleGameplayTags& Get() {return GameplayTags;}
	static void InitializeNativeTags();



	/*
	 *
	 * 아래의 GameplayTag는 초기화 과정 단계임
	 * - GameInstance의 초기화 과정에 UGameFrameworkComponentManager의 RegisterInitState에 등록되어 리니어 하게 업데이트 됨. 
	 * - 게임의 Actor 사이에 공유되며, GameFrameworkInitStateInterface를 상속 받은 클래스는 InitState를 상태머신처럼 사용하여 관리할 수 있도록 인터페이스를 제공 받음.
	 */
public:

	FGameplayTag Ability_Behavior_SurvivesDeath;
	
	FGameplayTag InitState_Spawned;
	FGameplayTag InitState_DataAvailable;
	FGameplayTag InitState_DataInitialized;
	FGameplayTag InitState_GameplayReady;

	/*
	 *	GA
	 */

	FGameplayTag GameplayEvent_Death;
	FGameplayTag GameplayEvent_RequestReset;
	FGameplayTag GameplayEvent_Reset;

	FGameplayTag Status_Death;
	FGameplayTag Status_Death_Dying;
	FGameplayTag Status_Death_Dead;
	
	/*
	 *
	 *	Enhanced Input Tags
	 *
	 */
	
	FGameplayTag InputTag_Move;
	FGameplayTag InputTag_Look_Mouse;

protected:

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags(UGameplayTagsManager& Manager);
	
	
private:
	static FBattleGameplayTags GameplayTags;
	
};