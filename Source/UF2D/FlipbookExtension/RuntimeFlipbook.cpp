// Fill out your copyright notice in the Description page of Project Settings.


#include "RuntimeFlipbook.h"
#include "PaperSprite.h"




URuntimeFlipbook::URuntimeFlipbook()
{
	KeyFrames.Empty();
	RuntimeKeyFrames.Empty();
}


FString URuntimeFlipbook::GetKeyFrameNotifyTag(int32 KeyFrameIndex) const
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		return RuntimeKeyFrames[KeyFrameIndex].NotifyTag;
	}
	return INVALID_KEY_FRAME_TAG;
}

bool URuntimeFlipbook::KeyFrameCanFireNotifyEvent(int32 KeyFrameIndex) const
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		return RuntimeKeyFrames[KeyFrameIndex].bCanFireNotifyEvent;
	}
	return false;
}

bool URuntimeFlipbook::SetFromPaperFlipbook(UPaperFlipbook* PaperFlipbook)
{
	if (PaperFlipbook != nullptr)
	{
		FramesPerSecond = PaperFlipbook->GetFramesPerSecond();
		CollisionSource = PaperFlipbook->GetCollisionSource();
		DefaultMaterial = PaperFlipbook->GetDefaultMaterial();

		KeyFrames.Empty();
		RuntimeKeyFrames.Empty();

		const int32 NumPaperKeyFrames = PaperFlipbook->GetNumKeyFrames();

		for (int32 PaperKeyFrameIndex = 0; PaperKeyFrameIndex < NumPaperKeyFrames; ++PaperKeyFrameIndex)
		{
			const FPaperFlipbookKeyFrame PaperFlipbookKeyFrame = PaperFlipbook->GetKeyFrameChecked(PaperKeyFrameIndex);
			const FRuntimeFlipbookKeyFrame RuntimeFlipbookKeyFrame = ConvertToRuntimeFlipbookKeyFrame(PaperFlipbookKeyFrame);

			KeyFrames.Add(PaperFlipbookKeyFrame);
			RuntimeKeyFrames.Add(RuntimeFlipbookKeyFrame);
		}

		InvalidateCachedData();

		return true;
	}

	return false;
}

bool URuntimeFlipbook::SetFromListOfKeyFrames(const TArray<FRuntimeFlipbookKeyFrame>& NewRuntimeKeyFrames, TEnumAsByte<EFlipbookCollisionMode::Type> NewCollisionSource, float NewFramesPerSecond /*= 15.0f*/)
{
	if (NewRuntimeKeyFrames.Num() > 0)
	{
		FramesPerSecond = FMath::Clamp<float>(NewFramesPerSecond, 0.0f, 1000.0f);
		CollisionSource = NewCollisionSource;

		KeyFrames.Empty();
		RuntimeKeyFrames.Empty();


		const int32 NumRuntimeKeyFrames = NewRuntimeKeyFrames.Num();

		for (int32 RuntimeKeyFrameIndex = 0; RuntimeKeyFrameIndex < NumRuntimeKeyFrames; ++RuntimeKeyFrameIndex)
		{
			const FPaperFlipbookKeyFrame PaperFlipbookKeyFrame = ConvertToPaperFlipbookKeyFrame(NewRuntimeKeyFrames[RuntimeKeyFrameIndex]);

			KeyFrames.Add(PaperFlipbookKeyFrame);
			RuntimeKeyFrames.Add(NewRuntimeKeyFrames[RuntimeKeyFrameIndex]);
		}

		InvalidateCachedData();

		return true;
	}

	return false;
}

void URuntimeFlipbook::SetFramesPerSecond(float NewFramesPerSecond)
{
	FramesPerSecond = FMath::Clamp<float>(NewFramesPerSecond, 0.0f, 1000.0f);
	InvalidateCachedData();
}

void URuntimeFlipbook::SetCollisionSource(TEnumAsByte<EFlipbookCollisionMode::Type> NewCollisionSource)
{
	CollisionSource = NewCollisionSource;
	InvalidateCachedData();
}

void URuntimeFlipbook::SetDefaultMaterial(UMaterialInterface* NewMaterial)
{
	DefaultMaterial = NewMaterial;
	InvalidateCachedData();
}

bool URuntimeFlipbook::SetSpriteAtKeyFrame(int32 KeyFrameIndex, UPaperSprite* NewSprite)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		RuntimeKeyFrames[KeyFrameIndex].Sprite = NewSprite;
		KeyFrames[KeyFrameIndex].Sprite = NewSprite;
		InvalidateCachedData();
		return true;
	}
	return false;
}

bool URuntimeFlipbook::SetNotifyAtKeyFrame(int32 KeyFrameIndex, bool bCanFireNotifyEvent, const FString& NewNotifyTag)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		RuntimeKeyFrames[KeyFrameIndex].bCanFireNotifyEvent = bCanFireNotifyEvent;
		RuntimeKeyFrames[KeyFrameIndex].NotifyTag = NewNotifyTag;
		InvalidateCachedData();
		return true;
	}
	return false;
}

bool URuntimeFlipbook::SetFrameRunAtKeyFrame(int32 KeyFrameIndex, int32 NewFrameRun)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		const int32 NewFrameRunClamped = FMath::Clamp<int32>(NewFrameRun, 1, 1000);
		RuntimeKeyFrames[KeyFrameIndex].FrameRun = NewFrameRunClamped;
		KeyFrames[KeyFrameIndex].FrameRun = NewFrameRunClamped;
		InvalidateCachedData();
		return true;
	}
	return false;
}

void URuntimeFlipbook::ClearKeyFrames()
{
	RuntimeKeyFrames.Empty();
	KeyFrames.Empty();
	InvalidateCachedData();
}

bool URuntimeFlipbook::SetRuntimeKeyFrame(int32 KeyFrameIndex, const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		const int32 NewFrameRunClamped = FMath::Clamp<int32>(NewRuntimeKeyFrame.FrameRun, 1, 1000);
		RuntimeKeyFrames[KeyFrameIndex] = NewRuntimeKeyFrame;
		RuntimeKeyFrames[KeyFrameIndex].FrameRun = NewFrameRunClamped;
		KeyFrames[KeyFrameIndex].Sprite = NewRuntimeKeyFrame.Sprite;
		KeyFrames[KeyFrameIndex].FrameRun = NewFrameRunClamped;
		InvalidateCachedData();
		return true;
	}
	return false;
}

bool URuntimeFlipbook::RemoveRuntimeKeyFrame(int32 KeyFrameIndex)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		RuntimeKeyFrames.RemoveAt(KeyFrameIndex);
		KeyFrames.RemoveAt(KeyFrameIndex);
		InvalidateCachedData();
		return true;
	}
	return false;
}

int32 URuntimeFlipbook::InsertNewRuntimeKeyFrameAtStart(const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame)
{
	const FPaperFlipbookKeyFrame NewPaperKeyFrame = ConvertToPaperFlipbookKeyFrame(NewRuntimeKeyFrame);
	KeyFrames.Insert(NewPaperKeyFrame, 0);
	const int32 Index = RuntimeKeyFrames.Insert(NewRuntimeKeyFrame, 0);
	InvalidateCachedData();
	return Index;
}

int32 URuntimeFlipbook::InsertNewRuntimeKeyFrameAtEnd(const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame)
{
	const FPaperFlipbookKeyFrame NewPaperKeyFrame = ConvertToPaperFlipbookKeyFrame(NewRuntimeKeyFrame);
	KeyFrames.Add(NewPaperKeyFrame);
	const int32 Index = RuntimeKeyFrames.Add(NewRuntimeKeyFrame);
	InvalidateCachedData();
	return Index;
}

bool URuntimeFlipbook::InsertNewRuntimeKeyFrameAtIndex(int32 Index, const FRuntimeFlipbookKeyFrame& NewRuntimeKeyFrame)
{
	if (RuntimeKeyFrames.IsValidIndex(Index))
	{
		const FPaperFlipbookKeyFrame NewPaperKeyFrame = ConvertToPaperFlipbookKeyFrame(NewRuntimeKeyFrame);
		KeyFrames.Insert(NewPaperKeyFrame, Index);
		RuntimeKeyFrames.Insert(NewRuntimeKeyFrame, Index);
		InvalidateCachedData();
		return true;
	}
	else if (Index == 0 && RuntimeKeyFrames.Num() == 0)
	{
		const FPaperFlipbookKeyFrame NewPaperKeyFrame = ConvertToPaperFlipbookKeyFrame(NewRuntimeKeyFrame);
		KeyFrames.Add(NewPaperKeyFrame);
		RuntimeKeyFrames.Add(NewRuntimeKeyFrame);
		InvalidateCachedData();
		return true;
	}
	return false;
}

FRuntimeFlipbookKeyFrame URuntimeFlipbook::GetRuntimeKeyFrame(int32 KeyFrameIndex, bool& bIsValidIndex)
{
	if (RuntimeKeyFrames.IsValidIndex(KeyFrameIndex))
	{
		bIsValidIndex = true;
		return RuntimeKeyFrames[KeyFrameIndex];
	}

	bIsValidIndex = false;
	return FRuntimeFlipbookKeyFrame();
}

FPaperFlipbookKeyFrame URuntimeFlipbook::ConvertToPaperFlipbookKeyFrame(const FRuntimeFlipbookKeyFrame& RuntimeFlipbookKeyFrame) const
{
	FPaperFlipbookKeyFrame NewPaperFlipbookKeyFrame = FPaperFlipbookKeyFrame();

	NewPaperFlipbookKeyFrame.Sprite = RuntimeFlipbookKeyFrame.Sprite;
	NewPaperFlipbookKeyFrame.FrameRun = FMath::Clamp<int32>(RuntimeFlipbookKeyFrame.FrameRun, 1, 1000);

	return NewPaperFlipbookKeyFrame;
}

FRuntimeFlipbookKeyFrame URuntimeFlipbook::ConvertToRuntimeFlipbookKeyFrame(const FPaperFlipbookKeyFrame& PaperFlipbookKeyFrame) const
{
	FRuntimeFlipbookKeyFrame NewRuntimeFlipbookKeyFrame = FRuntimeFlipbookKeyFrame();

	NewRuntimeFlipbookKeyFrame.Sprite = PaperFlipbookKeyFrame.Sprite;
	NewRuntimeFlipbookKeyFrame.FrameRun = FMath::Clamp<int32>(PaperFlipbookKeyFrame.FrameRun, 1, 1000);
	NewRuntimeFlipbookKeyFrame.bCanFireNotifyEvent = false;
	NewRuntimeFlipbookKeyFrame.NotifyTag = NONE_KEY_FRAME_TAG;

	return NewRuntimeFlipbookKeyFrame;
}
