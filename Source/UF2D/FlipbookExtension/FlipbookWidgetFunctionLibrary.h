// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "FlipbookWidgetFunctionLibrary.generated.h"









USTRUCT(BlueprintType, meta = (ShowOnlyInnerProperties))
struct FFlipbookWidgetData
{
	GENERATED_USTRUCT_BODY()

public:

	FFlipbookWidgetData();

	/** Flipbook currently being played */
	UPROPERTY(Category = "Flipbook Widget", EditAnywhere,meta = (DisplayThumbnail = "true"))
		UPaperFlipbook* Flipbook;

	/** Current play rate of the flipbook */
	UPROPERTY(Category = "Flipbook Widget", EditAnywhere)
		float PlayRate;

	/** Whether the flipbook should loop when it reaches the end, or stop */
	UPROPERTY()
		uint32 bLooping : 1;

	/** If playback should move the current position backwards instead of forwards */
	UPROPERTY()
		uint32 bReversePlayback : 1;

	/** Are we currently playing (moving Position) */
	UPROPERTY()
		uint32 bPlaying : 1;

	/** Current position in the timeline */
	UPROPERTY()
		float AccumulatedTime;

	/** Last frame index calculated */
	UPROPERTY()
		int32 CachedFrameIndex;

	void CalculateCurrentFrame();
	UPaperSprite* GetSpriteAtCachedIndex() const;
	void TickFlipbook(float DeltaTime);
};



UENUM(BlueprintType)
enum class EFlipbookWidgetEditModeUpdateRate : uint8
{
	FPS_25 	UMETA(DisplayName = "25 Fps"),
	FPS_30 	UMETA(DisplayName = "30 Fps"),
	FPS_60 	UMETA(DisplayName = "60 Fps")
};





UCLASS()
class UF2D_API UFlipbookWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:


	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static FSlateBrush CreateBrushFromSprite(UPaperSprite* Sprite, int32 Width = 0, int32 Height = 0);

	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static float GetDeltaTime(EFlipbookWidgetEditModeUpdateRate UpdateRateMode);


	/** Change the flipbook used by this instance (will reset the play time to 0 if it is a new flipbook). */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static bool SetFlipbook(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, UPaperFlipbook* NewFlipbook);

	/** Gets the flipbook used by this instance. */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static UPaperFlipbook* GetFlipbook(const  FFlipbookWidgetData& FlipbookAnimation);

	/** Start playback of flipbook */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void Play(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation);

	/** Start playback of flipbook from the start */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void PlayFromStart(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation);

	/** Start playback of flipbook in reverse */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void Reverse(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation);

	/** Start playback of flipbook in reverse from the end */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void ReverseFromEnd(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation);

	/** Stop playback of flipbook */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void Stop(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation);

	/** Get whether this flipbook is playing or not. */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static bool IsPlaying(const FFlipbookWidgetData& FlipbookAnimation);

	/** Get whether we are reversing or not */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static bool IsReversing(const FFlipbookWidgetData& FlipbookAnimation);

	/** Jump to a position in the flipbook (expressed in frames). If bFireEvents is true, event functions will fire, otherwise they will not. */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void SetPlaybackPositionInFrames(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, int32 NewFramePosition, bool bFireEvents);

	/** Get the current playback position (in frames) of the flipbook */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static int32 GetPlaybackPositionInFrames(const FFlipbookWidgetData& FlipbookAnimation);

	/** Jump to a position in the flipbook (expressed in seconds). If bFireEvents is true, event functions will fire, otherwise they will not. */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void SetPlaybackPosition(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, float NewPosition, bool bFireEvents);

	/** Get the current playback position (in seconds) of the flipbook */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static float GetPlaybackPosition(const FFlipbookWidgetData& FlipbookAnimation);

	/** true means we should loop, false means we should not. */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void SetLooping(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, bool bNewLooping);

	/** Get whether we are looping or not */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static bool IsLooping(const FFlipbookWidgetData& FlipbookAnimation);

	/** Sets the new play rate for this flipbook */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void SetPlayRate(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, float NewRate);

	/** Get the current play rate for this flipbook */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static float GetPlayRate(const FFlipbookWidgetData& FlipbookAnimation);

	/** Set the new playback position time to use */
	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void SetNewTime(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, float NewTime);

	/** Get length of the flipbook (in seconds) */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static float GetFlipbookLength(const FFlipbookWidgetData& FlipbookAnimation);

	/** Get length of the flipbook (in frames) */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static int32 GetFlipbookLengthInFrames(const FFlipbookWidgetData& FlipbookAnimation);

	/** Get the nominal framerate that the flipbook will be played back at (ignoring PlayRate), in frames per second */
	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static float GetFlipbookFramerate(const FFlipbookWidgetData& FlipbookAnimation);

	UFUNCTION(BlueprintCallable, Category = "Flipbook Widget")
		static void TickAnimation(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, float DeltaTime, int32 Width, int32 Height, UPaperSprite*& CurrentFrameAsSprite, FSlateBrush& CurrentFrameAsBrush);

	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static UPaperSprite* GetSpriteAtTime(const FFlipbookWidgetData& FlipbookAnimation, float Time);

	UFUNCTION(BlueprintPure, Category = "Flipbook Widget")
		static UPaperSprite* GetSpriteAtFrame(const FFlipbookWidgetData& FlipbookAnimation, int FrameIndex);

};
