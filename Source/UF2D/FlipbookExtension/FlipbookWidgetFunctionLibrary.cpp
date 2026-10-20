// Fill out your copyright notice in the Description page of Project Settings.


#include "FlipbookWidgetFunctionLibrary.h"

#include "PaperSprite.h"
#include "Brushes/SlateNoResource.h"

#include "PaperFlipbook.h"
#include "PaperSprite.h"


FFlipbookWidgetData::FFlipbookWidgetData()
	: Flipbook(nullptr)
	, PlayRate(1.0f)
	, bLooping(true)
	, bReversePlayback(false)
	, bPlaying(true)
	, AccumulatedTime(0.0f)
	, CachedFrameIndex(INDEX_NONE)
{

}

UPaperSprite* FFlipbookWidgetData::GetSpriteAtCachedIndex() const
{
	UPaperSprite* SpriteToSend = nullptr;
	if ((Flipbook != nullptr) && Flipbook->IsValidKeyFrameIndex(CachedFrameIndex))
	{
		SpriteToSend = Flipbook->GetKeyFrameChecked(CachedFrameIndex).Sprite;
	}
	return SpriteToSend;
}

void FFlipbookWidgetData::CalculateCurrentFrame()
{
	const int32 LastCachedFrame = CachedFrameIndex;
	CachedFrameIndex = (Flipbook != nullptr) ? Flipbook->GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;

	if (CachedFrameIndex != LastCachedFrame)
	{
		// KeyFrame Changed !
	}
}

void FFlipbookWidgetData::TickFlipbook(float DeltaTime)
{
	bool bIsFinished = false;

	if (bPlaying)
	{
		const float TimelineLength = UFlipbookWidgetFunctionLibrary::GetFlipbookLength(*this);
		const float EffectiveDeltaTime = DeltaTime * (bReversePlayback ? (-PlayRate) : (PlayRate));

		float NewPosition = AccumulatedTime + EffectiveDeltaTime;

		if (EffectiveDeltaTime > 0.0f)
		{
			if (NewPosition > TimelineLength)
			{
				if (bLooping)
				{
					// If looping, play to end, jump to start, and set target to somewhere near the beginning.
					UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(*this, TimelineLength, true);
					UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(*this, 0.0f, false);

					if (TimelineLength > 0.0f)
					{
						while (NewPosition > TimelineLength)
						{
							NewPosition -= TimelineLength;
						}
					}
					else
					{
						NewPosition = 0.0f;
					}
				}
				else
				{
					// If not looping, snap to end and stop playing.
					NewPosition = TimelineLength;
					UFlipbookWidgetFunctionLibrary::Stop(*this);
					bIsFinished = true;
				}
			}
		}
		else
		{
			if (NewPosition < 0.0f)
			{
				if (bLooping)
				{
					// If looping, play to start, jump to end, and set target to somewhere near the end.
					UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(*this, 0.0f, true);
					UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(*this, TimelineLength, false);

					if (TimelineLength > 0.0f)
					{
						while (NewPosition < 0.0f)
						{
							NewPosition += TimelineLength;
						}
					}
					else
					{
						NewPosition = 0.0f;
					}
				}
				else
				{
					// If not looping, snap to start and stop playing.
					NewPosition = 0.0f;
					UFlipbookWidgetFunctionLibrary::Stop(*this);
					bIsFinished = true;
				}
			}
		}

		UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(*this, NewPosition, true);
	}

	// Notify user that the flipbook finished playing
	if (bIsFinished)
	{

	}
}



void UFlipbookWidgetFunctionLibrary::TickAnimation(UPARAM(ref) FFlipbookWidgetData& FlipbookAnimation, float DeltaTime, int32 Width, int32 Height, UPaperSprite*& CurrentFrameAsSprite, FSlateBrush& CurrentFrameAsBrush)
{
	// Advance time
	FlipbookAnimation.TickFlipbook(DeltaTime);

	// Update the frame and push it to the renderer if necessary
	FlipbookAnimation.CalculateCurrentFrame();

	CurrentFrameAsSprite = FlipbookAnimation.GetSpriteAtCachedIndex();

	CurrentFrameAsBrush = UFlipbookWidgetFunctionLibrary::CreateBrushFromSprite(CurrentFrameAsSprite, Width, Height);
}

UPaperSprite* UFlipbookWidgetFunctionLibrary::GetSpriteAtTime(const FFlipbookWidgetData& FlipbookAnimation, float Time)
{
	if (FlipbookAnimation.Flipbook != nullptr)
	{
		return FlipbookAnimation.Flipbook->GetSpriteAtTime(Time);
	}
	return nullptr;
}

UPaperSprite* UFlipbookWidgetFunctionLibrary::GetSpriteAtFrame(const FFlipbookWidgetData& FlipbookAnimation, int FrameIndex)
{
	if (FlipbookAnimation.Flipbook != nullptr)
	{
		return FlipbookAnimation.Flipbook->GetSpriteAtFrame(FrameIndex);
	}
	return nullptr;
}

bool UFlipbookWidgetFunctionLibrary::SetFlipbook(FFlipbookWidgetData& FlipbookAnimation, UPaperFlipbook* NewFlipbook)
{
	if (NewFlipbook != FlipbookAnimation.Flipbook)
	{
		FlipbookAnimation.Flipbook = NewFlipbook;
		return true;
	}

	return false;
}

UPaperFlipbook* UFlipbookWidgetFunctionLibrary::GetFlipbook(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.Flipbook;
}

void UFlipbookWidgetFunctionLibrary::Play(FFlipbookWidgetData& FlipbookAnimation)
{
	FlipbookAnimation.bReversePlayback = false;
	FlipbookAnimation.bPlaying = true;
}

void UFlipbookWidgetFunctionLibrary::PlayFromStart(FFlipbookWidgetData& FlipbookAnimation)
{
	SetPlaybackPosition(FlipbookAnimation, 0.0f, /*bFireEvents=*/ false);
	Play(FlipbookAnimation);
}

void UFlipbookWidgetFunctionLibrary::Reverse(FFlipbookWidgetData& FlipbookAnimation)
{
	FlipbookAnimation.bReversePlayback = true;
	FlipbookAnimation.bPlaying = true;
}

void UFlipbookWidgetFunctionLibrary::ReverseFromEnd(FFlipbookWidgetData& FlipbookAnimation)
{
	SetPlaybackPosition(FlipbookAnimation, GetFlipbookLength(FlipbookAnimation), /*bFireEvents=*/ false);
	Reverse(FlipbookAnimation);
}

void UFlipbookWidgetFunctionLibrary::Stop(FFlipbookWidgetData& FlipbookAnimation)
{
	FlipbookAnimation.bPlaying = false;
}

bool UFlipbookWidgetFunctionLibrary::IsPlaying(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.bPlaying;
}

bool UFlipbookWidgetFunctionLibrary::IsReversing(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.bPlaying && FlipbookAnimation.bReversePlayback;
}

void UFlipbookWidgetFunctionLibrary::SetPlaybackPositionInFrames(FFlipbookWidgetData& FlipbookAnimation, int32 NewFramePosition, bool bFireEvents)
{
	const float Framerate = GetFlipbookFramerate(FlipbookAnimation);
	const float NewTime = (Framerate > 0.0f) ? (NewFramePosition / Framerate) : 0.0f;
	SetPlaybackPosition(FlipbookAnimation, NewTime, bFireEvents);
}

int32 UFlipbookWidgetFunctionLibrary::GetPlaybackPositionInFrames(const FFlipbookWidgetData& FlipbookAnimation)
{
	const float Framerate = GetFlipbookFramerate(FlipbookAnimation);
	const int32 NumFrames = GetFlipbookLengthInFrames(FlipbookAnimation);
	if (NumFrames > 0)
	{
		return FMath::Clamp<int32>(FMath::TruncToInt(FlipbookAnimation.AccumulatedTime * Framerate), 0, NumFrames - 1);
	}
	else
	{
		return 0;
	}
}

void UFlipbookWidgetFunctionLibrary::SetPlaybackPosition(FFlipbookWidgetData& FlipbookAnimation, float NewPosition, bool bFireEvents)
{
	float OldPosition = FlipbookAnimation.AccumulatedTime;
	FlipbookAnimation.AccumulatedTime = NewPosition;

	// If we should be firing events for this track...
	if (bFireEvents)
	{
		float MinTime;
		float MaxTime;
		if (!FlipbookAnimation.bReversePlayback)
		{
			// If playing sequence forwards.
			MinTime = OldPosition;
			MaxTime = FlipbookAnimation.AccumulatedTime;

			// Slight hack here.. if playing forwards and reaching the end of the sequence, force it over a little to ensure we fire events actually on the end of the sequence.
			if (MaxTime == GetFlipbookLength(FlipbookAnimation))
			{
				MaxTime += (float)KINDA_SMALL_NUMBER;
			}
		}
		else
		{
			// If playing sequence backwards.
			MinTime = FlipbookAnimation.AccumulatedTime;
			MaxTime = OldPosition;

			// Same small hack as above for backwards case.
			if (MinTime == 0.0f)
			{
				MinTime -= (float)KINDA_SMALL_NUMBER;
			}
		}
	}

	if (OldPosition != FlipbookAnimation.AccumulatedTime)
	{
		FlipbookAnimation.CalculateCurrentFrame();
	}
}

float UFlipbookWidgetFunctionLibrary::GetPlaybackPosition(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.AccumulatedTime;
}

void UFlipbookWidgetFunctionLibrary::SetLooping(FFlipbookWidgetData& FlipbookAnimation, bool bNewLooping)
{
	FlipbookAnimation.bLooping = bNewLooping;
}

bool UFlipbookWidgetFunctionLibrary::IsLooping(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.bLooping;
}

void UFlipbookWidgetFunctionLibrary::SetPlayRate(FFlipbookWidgetData& FlipbookAnimation, float NewRate)
{
	FlipbookAnimation.PlayRate = NewRate;
}

float UFlipbookWidgetFunctionLibrary::GetPlayRate(const FFlipbookWidgetData& FlipbookAnimation)
{
	return FlipbookAnimation.PlayRate;
}

void UFlipbookWidgetFunctionLibrary::SetNewTime(FFlipbookWidgetData& FlipbookAnimation, float NewTime)
{
	SetPlaybackPosition(FlipbookAnimation, NewTime, /*bFireEvents=*/ false);
}

float UFlipbookWidgetFunctionLibrary::GetFlipbookLength(const FFlipbookWidgetData& FlipbookAnimation)
{
	return (FlipbookAnimation.Flipbook != nullptr) ? FlipbookAnimation.Flipbook->GetTotalDuration() : 0.0f;
}

int32 UFlipbookWidgetFunctionLibrary::GetFlipbookLengthInFrames(const FFlipbookWidgetData& FlipbookAnimation)
{
	return (FlipbookAnimation.Flipbook != nullptr) ? FlipbookAnimation.Flipbook->GetNumFrames() : 0;
}

float UFlipbookWidgetFunctionLibrary::GetFlipbookFramerate(const FFlipbookWidgetData& FlipbookAnimation)
{
	return (FlipbookAnimation.Flipbook != nullptr) ? FlipbookAnimation.Flipbook->GetFramesPerSecond() : 15.0f;
}

FSlateBrush UFlipbookWidgetFunctionLibrary::CreateBrushFromSprite(UPaperSprite* Sprite, int32 Width /*= 0*/, int32 Height /*= 0*/)
{
	if (Sprite != nullptr)
	{
		const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
		const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();

		FSlateBrush Brush;
		Brush.SetResourceObject(Sprite);
		Width = (Width > 0) ? Width : SpriteSize.X;
		Height = (Height > 0) ? Height : SpriteSize.Y;
		Brush.ImageSize = FVector2D(Width, Height);
		return Brush;
	}

	return FSlateNoResource();
}

float UFlipbookWidgetFunctionLibrary::GetDeltaTime(EFlipbookWidgetEditModeUpdateRate UpdateRateMode)
{
	switch (UpdateRateMode)
	{
	case EFlipbookWidgetEditModeUpdateRate::FPS_25:
		return (1.0f / 25.0f);
	case EFlipbookWidgetEditModeUpdateRate::FPS_30:
		return (1.0f / 30.0f);
	case EFlipbookWidgetEditModeUpdateRate::FPS_60:
		return (1.0f / 60.0f);
	}

	return 0.02f;
}

