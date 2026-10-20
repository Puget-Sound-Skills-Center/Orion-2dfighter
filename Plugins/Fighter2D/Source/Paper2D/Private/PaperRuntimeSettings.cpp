// Copyright Epic Games, Inc. All Rights Reserved.

#include "PaperRuntimeSettings.h"

#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "GameFramework/Character.h"

//////////////////////////////////////////////////////////////////////////
// UPaperRuntimeSettings

UPaperRuntimeSettings::UPaperRuntimeSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bEnableSpriteAtlasGroups(false)
	, bEnableTerrainSplineEditing(false)
	, bResizeSpriteDataToMatchTextures(true)
{

	AsyncLoadingClasses.AddUnique(UTexture2D::StaticClass());
	AsyncLoadingClasses.AddUnique(UPaperSprite::StaticClass());
	AsyncLoadingClasses.AddUnique(UPaperFlipbook::StaticClass());
	AsyncLoadingClasses.AddUnique(ACharacter::StaticClass());

	AsyncScanPaths.AddUnique("/Game");
}
