#pragma once

#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"

class UClass;

class FAssetTypeActions_BattleContextEffectsLibrary : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override {return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_BattleContextEffectsLibrary", "BattleContextEffectsLibrary");}
	virtual FColor GetTypeColor() const override {return FColor(65,200,98);}
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Gameplay;}
	
};

