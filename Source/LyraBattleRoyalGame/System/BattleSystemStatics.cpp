#include "BattleSystemStatics.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleSystemStatics)

void UBattleSystemStatics::PlayNextGame(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}

	const FWorldContext& WorldContext = GEngine->GetWorldContextFromWorldChecked(World);
	FURL LastURL = WorldContext.LastURL;

	FString URL = LastURL.ToString();
	URL.RemoveFromStart(LastURL.GetHostPortString());

	const bool bAbsolute = false;
	const bool bShouldSkipGameNotify = false;
	World->ServerTravel(URL, bAbsolute, bShouldSkipGameNotify);	
}

void UBattleSystemStatics::SetScalarParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName,
	const float ParameterValue, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent<UMeshComponent>(bIncludeChildActors, [=](UMeshComponent* InComponent)
		{
			InComponent->SetScalarParameterValueOnMaterials(ParameterName, ParameterValue);
		});
	}
}

void UBattleSystemStatics::SetVectorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName,
	const FVector ParameterValue, bool bIncludeChildActors)
{
	if (TargetActor != nullptr)
	{
		TargetActor->ForEachComponent<UMeshComponent>(bIncludeChildActors, [=](UMeshComponent* InComponent)
		{
			InComponent->SetVectorParameterValueOnMaterials(ParameterName, ParameterValue);
		});
	}
}

void UBattleSystemStatics::SetColorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName,
	const FLinearColor ParameterValue, bool bIncludeChildActors)
{
	SetVectorParameterValueOnAllMeshComponents(TargetActor, ParameterName, FVector(ParameterValue), bIncludeChildActors);
}

TArray<UActorComponent*> UBattleSystemStatics::FindComponentsByClass(AActor* TargetActor,
                                                                     TSubclassOf<UActorComponent> ComponentClass, bool bIncludeChildActors)
{
	TArray<UActorComponent*> Components;
	if (TargetActor != nullptr)
	{
		TargetActor->GetComponents(ComponentClass, Components, bIncludeChildActors);
	}

	return MoveTemp(Components);
}
