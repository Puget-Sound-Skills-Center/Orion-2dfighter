// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RuntimeFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "RuntimeFlipbookComponent.generated.h"



/* Flipbook Notify Event Delegate  */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlipbookNotifyEvent, FString, NotifyTag, int32, KeyFrameIndex);


/**
 * 
 */
UCLASS(ShowCategories = (Mobility, ComponentReplication), ClassGroup = "2D Runtime Flipbook", meta = (BlueprintSpawnableComponent))
class UF2D_API URuntimeFlipbookComponent : public UPaperFlipbookComponent
{
	GENERATED_BODY()

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** Change the flipbook used by this instance (will reset the play time to 0 if it is a new flipbook). */
	UFUNCTION(BlueprintCallable, Category = "Components|RuntimeFlipbookComponent")
		virtual bool SetRuntimeFlipbook(class URuntimeFlipbook* NewRuntimeFlipbook);

	UFUNCTION(BlueprintCallable, Category = "Components|RuntimeFlipbookComponent")
		URuntimeFlipbook* GetRuntimeSourceFlipbook(bool& bIsValid) const;

	UFUNCTION(BlueprintCallable, Category = "Components|RuntimeFlipbookComponent")
		bool ForceRefreshComponent (bool bResetTimeAndFrame = true, bool bRefreshMaterial = false);

	UPROPERTY(BlueprintAssignable)
		FOnFlipbookNotifyEvent OnFlipbookNotifyEvent;

private:
	int32 GetLastFrameIndex() const;
	void FireNotifyEvents(int32 LastFrameIndex);
};
