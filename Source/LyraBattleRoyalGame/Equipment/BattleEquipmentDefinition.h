#pragma once

#include "Containers/Array.h"
#include "Math/Transform.h"
#include "Templates/SubclassOf.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "BattleEquipmentDefinition.generated.h"

class UBattleAbilitySet;
class UBattleEquipmentInstance;

USTRUCT()
struct FBattleEquipmentActorToSpawn
{
	GENERATED_BODY()

	// Spawn할 대상 Actor 클래스 (== Actor를 상속받은 Asset)
	UPROPERTY(EditAnywhere, Category= Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	// 어느 Bone Socket에 붙일지 결정
	UPROPERTY(EditAnywhere, Category= Equipment)
	FName AttachSocket;
	
	// Socket에 어느정도 Transformation을 더할지 결정
	// Rotation, Position, Scale
	UPROPERTY(EditAnywhere, Category= Equipment)
	FTransform AttachTransform;
	
};


/*
 *  Blueprintable => 해당 클래스를 상속받는 모든 클래스는 Blueprint로 정의할 수 있다.
 *	BlueprintType => BP에서 해당 클래스를 변수로서 사용 가능하다.
 */


// EquipmentDefinition은 장착 아이템에 대한 정의 클래스(메타 데이터)임
UCLASS(BlueprintType, Blueprintable)
class UBattleEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UBattleEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 해당 메타 데이터를 사용하면, 어떤 인스턴스를 Spawn할지 결정하는 클래스
	UPROPERTY(EditDefaultsOnly, Category= Equipment)
	TSubclassOf<UBattleEquipmentInstance> InstanceType;

	// 해당 장착 아이템을 사용하면 어떤 Actor가 Spawn 되는지 정보를 담고 있음.
	UPROPERTY(EditDefaultsOnly, Category= Equipment)
	TArray<FBattleEquipmentActorToSpawn> ActorsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category= Equipment)
	TArray<TObjectPtr<UBattleAbilitySet>> AbilitySetsToGrant;
	
};



