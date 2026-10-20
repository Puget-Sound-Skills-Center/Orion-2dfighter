// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPStatics.generated.h"

/**
 * 
 */
UCLASS()
class UF2D_API UBPStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Call by Function Name", DefaultToSelf = "Object"), Category = "FighterPlugin")
		static bool CallByNameFunction(UObject* Object, FString FunctionName);
};
