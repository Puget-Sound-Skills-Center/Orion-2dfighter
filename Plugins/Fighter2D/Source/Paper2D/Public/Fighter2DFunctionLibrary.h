// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PaperFlipbookComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpriteEditorOnlyTypes.h"


#include "Fighter2DFunctionLibrary.generated.h"
class UPaperSprite;
/**
 * 
 */
/*Shows SHape Role */
UENUM(BlueprintType)
enum EShapeRoleType
{
	Collision,HitBox,GuardBox,TraceBox,CustomBox
};

USTRUCT(BlueprintType)
struct FSpriteShapeBP
{
	FSpriteGeometryShape Shape;
	UPROPERTY()
	UPaperSprite* Owner=nullptr;
	GENERATED_BODY()


	
	
};

UENUM(BlueprintType)
enum EShapeQuerySettings
{
	HitBoxes,GuardBoxes,TraceBoxes,CustomBoxes,AllBoxes
};


UCLASS()
class PAPER2D_API UFighter2DFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	/*"All" Switch will return anybox available including Collisions*/
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static TArray<FSpriteShapeBP> GetShapeListFromSprite(UPaperSprite* Sprite,TEnumAsByte<EShapeQuerySettings> Type);
/*Get Shape Type*/
	UFUNCTION(BlueprintCallable,BlueprintPure)
    static TEnumAsByte<EShapeRoleType> GetShapeType(FSpriteShapeBP InShape);
/*Get Box Size */
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FVector2D GetBoxSize(FSpriteShapeBP InShape);
	/*Box Position Relative To Pivot*/
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FVector2D GetBoxPosition( FSpriteShapeBP InShape);
	/*Returns All Vertices From a shape . For Advanced Users*/
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static TArray<FVector2D> GetVerticesFromShape(FSpriteShapeBP InShape);
	UFUNCTION(BlueprintCallable)
	static void SetTextureOverride(UPaperFlipbook* PaperFlipBook,int Input);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static void GetAllSprites(TArray<UPaperSprite*>& PaperSprites,UPaperFlipbook* Flipbook);
};
