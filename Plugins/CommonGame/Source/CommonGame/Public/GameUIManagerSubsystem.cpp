#include "GameUIManagerSubsystem.h"

#include "GameUIPolicy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIManagerSubsystem)

UGameUIManagerSubsystem::UGameUIManagerSubsystem()
	: Super()
{
}

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentPolicy != InPolicy)
	{
		CurrentPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CurrentPolicy && !DefaultUIPolicyClass.IsNull())
	{
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();

		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 데디 서버는 UISubsystem을 활성화하지 않음.
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		// UGameUIManagerSubsystem을 상속 받은 Subsystem이 없다면, 이를 활성화하지 않음.
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}
	
	return false;
}

void UGameUIManagerSubsystem::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerAdded(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerRemoved(LocalPlayer);
	}
}

void UGameUIManagerSubsystem::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	if (ensure(LocalPlayer) && CurrentPolicy)
	{
		CurrentPolicy->NotifyPlayerDestroyed(LocalPlayer);
	}
}

