#include "BattleGameplayTags.h"
#include "BattleLogChannels.h"
#include "GameplayTagsManager.h"

FBattleGameplayTags FBattleGameplayTags::GameplayTags;


void FBattleGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
	
}

void FBattleGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native)")) + FString(TagComment)); 
}

void FBattleGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "Survive");
	
	/*
	 * GameFrameworkComponentManager InitState tags
	 */
	AddTag(InitState_Spawned, "InitState.Spawned", "1: Actor/Component has initially spawned and can be extended");
	AddTag(InitState_DataAvailable, "InitState.DataAvailable","2: All required data has been loaded/replicated and is ready for initialization");
	AddTag(InitState_DataInitialized, "InitState.DataInitialized","3: The available data has initialized for this actor/component, but it is not ready for full gameplay");
	AddTag(InitState_GameplayReady, "InitState.GameplayReady","4: The actor/component is fully ready for active gameplay");

	/*
	 *  GA
	 */

	AddTag(GameplayEvent_Death, "GameplayEvent.Death", "Death Event");
	AddTag(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Reset Request");
	AddTag(GameplayEvent_Reset, "GameplayEvent.Reset", "Reset Ended");

	AddTag(Status_Death, "Status.Death", "Target has the death status.");
	AddTag(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	AddTag(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");
	
	/*
	*	EnhancedInput Tags
	*/
	AddTag(InputTag_Move, "InputTag.Move", "");
	AddTag(InputTag_Look_Mouse, "InputTag.Look.Mouse", "");
	AddTag(InputTag_Crouch, "InputTag.Crouch", "");
	AddTag(InputTag_Interact, "InputTag.Interact", "");
	
	
}
