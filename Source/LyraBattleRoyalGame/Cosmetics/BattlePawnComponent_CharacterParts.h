#pragma once

#include "Components/PawnComponent.h"
#include "BattleCharacterPartTypes.h"
#include "BattleCosmeticAnimationTypes.h"
#include "BattlePawnComponent_CharacterParts.generated.h"

// 인스턴스화된 Character Part의 단위
USTRUCT()
struct FBattleAppliedCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part의 정의
	UPROPERTY()
	FBattleCharacterPart Part;

	// CharacterPartList에서 할당 받은 Part 핸들값
	// Controller쪽 PartEntry의 Handle 값과 같아야함 => 같은 장비면
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// 인스턴스화된 CharacterPart용 Actor
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
	
};

// 실질적인 CharacterParts를 관리하는 클래스
USTRUCT(BlueprintType)
struct FBattleCharacterPartList
{
	GENERATED_BODY()

	FBattleCharacterPartList()
		: OwnerComponent(nullptr)
	{}

	FBattleCharacterPartList(UBattlePawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{}
	
	FGameplayTagContainer CollectCombinedTags() const;
	
	bool SpawnActorForEntry(FBattleAppliedCharacterPartEntry& Entry);
	void DestroyActorForEntry(FBattleAppliedCharacterPartEntry& Entry);
	
	FBattleCharacterPartHandle AddEntry(FBattleCharacterPart NewPart);
	void RemoveEntry(FBattleCharacterPartHandle Handle);

	// 현재 인스턴스화된 Character Parts
	UPROPERTY()
	TArray<FBattleAppliedCharacterPartEntry> Entries;

	// 해당 CharacterPartsList의 Owner인 PawnComponent
	UPROPERTY()
	TObjectPtr<UBattlePawnComponent_CharacterParts> OwnerComponent;

	// PartHandle의 값을 할당, 관리하는 변수
	int32 PartHandleCounter = 0;
	
};


// PawnComponent로서, Character Parts를 인스턴스화하여 관리한다.
UCLASS()
class UBattlePawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()
public:
	UBattlePawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;
	
	USkeletalMeshComponent* GetParentMeshComponent() const;
	USceneComponent* GetSceneComponentToAttachTo() const;
	
	void BroadcastChanged();
	FBattleCharacterPartHandle AddCharacterPart(const FBattleCharacterPart& NewPart);

	void RemoveCharacterPart(FBattleCharacterPartHandle Handle);
	
	// 인스턴스화된 Character Parts의 List
	UPROPERTY()
	FBattleCharacterPartList CharacterPartList;

	// 애니메이션 적용을 위한 메시와의 연결고리
	UPROPERTY(EditAnywhere, Category=Cosmetics)
	FBattleAnimBodyStyleSelectionSet BodyMeshes;
};

