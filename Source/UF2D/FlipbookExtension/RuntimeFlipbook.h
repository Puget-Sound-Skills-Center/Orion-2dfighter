// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperFlipbook.h"
#include "FlipbookExtensionLibrary.h"
#include "RuntimeFlipbook.generated.h"


class UPaperSprite;

/**
 * 
 */
UCLASS()
class UF2D_API URuntimeFlipbook : public UPaperFlipbook
{
	GENERATED_BODY()

protected:

	UPROPERTY()
		TArray<FRuntimeFlipbookKeyFrame> RuntimeKeyFrames;

public:

	URuntimeFlipbook();

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetFromPaperFlipbook(UPaperFlipbook* PaperFlipbook);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetFromListOfKeyFrames(const TArray<FRuntimeFlipbookKeyFrame>& NewRuntimeKeyFrames, TEnumAsByte<EFlipbookCollisionMode::Type> NewCollisionSource, float NewFramesPerSecond = 15.0f);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		void SetFramesPerSecond(float NewFramesPerSecond = 15.0f);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		void SetCollisionSource(TEnumAsByte<EFlipbookCollisionMode::Type> NewCollisionSource);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		void SetDefaultMaterial(UMaterialInterface* NewMaterial);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetSpriteAtKeyFrame(int32 KeyFrameIndex, UPaperSprite* NewSprite);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetNotifyAtKeyFrame(int32 KeyFrameIndex, bool bCanFireNotifyEvent, const FString&  NewNotifyTag);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetFrameRunAtKeyFrame(int32 KeyFrameIndex, int32 NewFrameRun = 1);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		void ClearKeyFrames();

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool SetRuntimeKeyFrame(int32 KeyFrameIndex, const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool RemoveRuntimeKeyFrame(int32 KeyFrameIndex);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		int32 InsertNewRuntimeKeyFrameAtStart(const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		int32 InsertNewRuntimeKeyFrameAtEnd(const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool InsertNewRuntimeKeyFrameAtIndex(int32 Index, const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame);


	UFUNCTION(BlueprintPure, Category = "Runtime Flipbook")
		FRuntimeFlipbookKeyFrame GetRuntimeKeyFrame(int32 KeyFrameIndex, bool& bIsValidIndex);

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		FString GetKeyFrameNotifyTag(int32 KeyFrameIndex) const;

	UFUNCTION(BlueprintCallable, Category = "Runtime Flipbook")
		bool KeyFrameCanFireNotifyEvent(int32 KeyFrameIndex) const;


protected:

	FPaperFlipbookKeyFrame ConvertToPaperFlipbookKeyFrame(const FRuntimeFlipbookKeyFrame& RuntimeFlipbookKeyFrame) const;
	FRuntimeFlipbookKeyFrame ConvertToRuntimeFlipbookKeyFrame(const FPaperFlipbookKeyFrame& PaperFlipbookKeyFrame) const;
};
