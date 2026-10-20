// Fill out your copyright notice in the Description page of Project Settings.

#include "RuntimeFlipbookComponent.h"

#include "RuntimeFlipbook.h"
#include "ContentStreaming.h"




void URuntimeFlipbookComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	const int32 LastCachedFrameIndex = GetLastFrameIndex();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FireNotifyEvents(LastCachedFrameIndex);
}

bool URuntimeFlipbookComponent::SetRuntimeFlipbook(class  URuntimeFlipbook* NewRuntimeFlipbook)
{
	// Don't allow changing the sprite if we are "static".
	AActor* ComponentOwner = GetOwner();
	if ((ComponentOwner == nullptr) || AreDynamicDataChangesAllowed())
	{
		SourceFlipbook = NewRuntimeFlipbook;
		return ForceRefreshComponent();
	}

	return false;
}

URuntimeFlipbook* URuntimeFlipbookComponent::GetRuntimeSourceFlipbook(bool& bIsValid) const
{
	URuntimeFlipbook* RuntimeFlipbook = Cast<URuntimeFlipbook>(SourceFlipbook);
	bIsValid = (RuntimeFlipbook != nullptr);
	return (bIsValid ? RuntimeFlipbook : nullptr);
}

bool URuntimeFlipbookComponent::ForceRefreshComponent(bool bResetTimeAndFrame /*= true*/, bool bRefreshMaterial /*= false*/)
{
	URuntimeFlipbook* RuntimeFlipbook = Cast<URuntimeFlipbook>(SourceFlipbook);
	if (RuntimeFlipbook != nullptr && bRefreshMaterial == true)
	{
		SetMaterial(0, RuntimeFlipbook->GetDefaultMaterial());
	}

	if (AreDynamicDataChangesAllowed())
	{
		if (bResetTimeAndFrame)
		{
			// We need to also reset the frame and time also
			AccumulatedTime = 0.0f;
			CalculateCurrentFrame();
		}

		// Need to send this to render thread at some point
		MarkRenderStateDirty();

		// Update physics representation right away
		FlipbookChangedPhysicsState();

		// Notify the streaming system. Don't use Update(), because this may be the first time the mesh has been set
		// and the component may have to be added to the streaming system for the first time.
		IStreamingManager::Get().NotifyPrimitiveUpdated(this);

		// Since we have new mesh, we need to update bounds
		UpdateBounds();

		return true;
	}

	return false;
}

int32 URuntimeFlipbookComponent::GetLastFrameIndex() const
{
	if (CachedFrameIndex == INDEX_NONE && SourceFlipbook != nullptr)
	{
		return (PlayRate >= 0) ? SourceFlipbook->GetNumKeyFrames() - 1 : 0;
	}

	return  (SourceFlipbook != nullptr) ? SourceFlipbook->GetKeyFrameIndexAtTime(AccumulatedTime) : INDEX_NONE;
}

void URuntimeFlipbookComponent::FireNotifyEvents(int32 LastFrameIndex)
{
	const UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	// a hack to only fire notify events in play mode.
	if (World->WorldType == EWorldType::PIE || World->WorldType == EWorldType::Game)
	{
		if (URuntimeFlipbook* RuntimeFlipbook = Cast<URuntimeFlipbook>(SourceFlipbook))
		{
			if (RuntimeFlipbook->GetNumKeyFrames() > 0 && LastFrameIndex != INDEX_NONE && CachedFrameIndex != LastFrameIndex)
			{
				for (int32 KeyFrameIdx = 0; KeyFrameIdx < RuntimeFlipbook->GetNumKeyFrames(); KeyFrameIdx++)
				{
					if (!RuntimeFlipbook->KeyFrameCanFireNotifyEvent(KeyFrameIdx) || (CachedFrameIndex != KeyFrameIdx))
					{
						continue;
					}
					OnFlipbookNotifyEvent.Broadcast(RuntimeFlipbook->GetKeyFrameNotifyTag(KeyFrameIdx), KeyFrameIdx);
				}
			}
		}
	}
}
