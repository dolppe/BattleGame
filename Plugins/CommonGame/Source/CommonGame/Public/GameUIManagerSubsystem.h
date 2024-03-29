#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameUIManagerSubsystem.generated.h"


class UGameUIPolicy;
class UCommonLocalPlayer;

/*
 *	GameInstanceSubsystem임.
 *	- Abstract이기 때문에 이를 상속받은 Concrete Class로서 활용 해야함.
 *
 */

UCLASS(Abstract, Config=Game)
class COMMONGAME_API UGameUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGameUIManagerSubsystem();

	void SwitchToPolicy(UGameUIPolicy* InPolicy);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;


	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	// UI에 대한 정책을 결정할 객체
	UPROPERTY(Transient)
	TObjectPtr<UGameUIPolicy> CurrentPolicy = nullptr;
	
	// Default UI Policy를 생성할 class
	UPROPERTY(Config, EditAnywhere)
	TSoftClassPtr<UGameUIPolicy> DefaultUIPolicyClass;
	
 
};
