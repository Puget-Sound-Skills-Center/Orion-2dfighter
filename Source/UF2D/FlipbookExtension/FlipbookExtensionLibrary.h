// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PaperSprite.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FlipbookExtensionLibrary.generated.h"




#define NONE_KEY_FRAME_TAG TEXT("NONE")
#define INVALID_KEY_FRAME_TAG TEXT("INVALID")

class URuntimeFlipbook;


USTRUCT(BlueprintType)
struct FRuntimeFlipbookKeyFrame
{
public:
	GENERATED_USTRUCT_BODY()

		UPROPERTY(Category = "Runtime Flipbook", EditAnywhere, BlueprintReadWrite)
		UPaperSprite* Sprite;

	UPROPERTY(Category = "Runtime Flipbook", BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
		int32 FrameRun;

	UPROPERTY(Category = "Runtime Flipbook", EditAnywhere, BlueprintReadWrite)
		bool bCanFireNotifyEvent;

	UPROPERTY(Category = "Runtime Flipbook", EditAnywhere, BlueprintReadWrite)
		FString  NotifyTag;


	FRuntimeFlipbookKeyFrame()
		: Sprite(nullptr)
		, FrameRun(1)
		, bCanFireNotifyEvent(false)
		, NotifyTag(NONE_KEY_FRAME_TAG)
	{
	}
};
