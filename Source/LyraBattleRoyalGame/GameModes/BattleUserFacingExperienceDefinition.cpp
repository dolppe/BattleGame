#include "BattleUserFacingExperienceDefinition.h"
#include "CommonSessionSubsystem.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleUserFacingExperienceDefinition)

UCommonSession_HosetSessionRequest* UBattleUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	// - UCommonSession_HosetSessionRequest는 UObject로 생성해두고, 알아서 GC가 돈다.
	// - 해당 객체는 현재 프레임에서 사용하기에 GC에 대한 염려가 없음.
	// - 만약 다음 프레임이든, 추가 프레임 상에서 해당 객체를 사용할 경우 Lifetime 관리가 필요.
	// 그렇지 않으면 Dangling 남.
	
	UCommonSession_HosetSessionRequest* Result = NewObject<UCommonSession_HosetSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
