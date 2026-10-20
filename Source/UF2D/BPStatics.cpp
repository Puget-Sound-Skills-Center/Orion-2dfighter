// Fill out your copyright notice in the Description page of Project Settings.


#include "BPStatics.h"


bool UBPStatics::CallByNameFunction(UObject* Object, FString FunctionName)
{
	UFunction* Function = Object->FindFunction(*FunctionName);

	//debug checks
	bool bFound = (Object != nullptr && Function != nullptr);
	bool bObjectNull = (!bFound && Object == nullptr);
	bool bFunctionNull = (!bFound && Function == nullptr);

	if (bFound)
	{
		Object->ProcessEvent(Function, nullptr);
		return true;
	}

	if (bObjectNull)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Invalid or Missing Object Reference."));
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, "Object Does Not Exist!");
	}

	if (bFunctionNull)
	{
		UE_LOG(LogBlueprintUserMessages, Warning, TEXT("Function (%s) does not exist. Please make sure the function/event name is correct."), *FunctionName);
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, "Function " + FString(FunctionName) + " does not exist. Please make sure the function/event name is correct.");
	}

	return false;
}

