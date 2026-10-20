// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AsyncLoadByPath.generated.h"

struct FStreamableHandle;
class UAssetManagerSettings;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncAssetLoaded, const TArray<UObject*>&, Loaded);
/**
 * 
 */
USTRUCT(BlueprintType)
struct FAsyncClassQuery
{
public:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowAbstract="true"))
	UClass* Class=nullptr;
	//if this class has blueprints
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool HasBlueprints=false;
};
UCLASS()
class PAPER2D_API UAsyncLoadByPath : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	virtual void Activate() override;

	TArray<FName> Paths;
	TArray<FAsyncClassQuery> AdditionalClasses;
	bool Recursive;
	


	FDateTime BeginTime;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly="true", WorldContext = "WorldContextObject",AutoCreateRefTerm="LoadBundles, AdditionalClasses"))
	static UAsyncLoadByPath* AsyncLoadByPath(UObject* worldContextObject,TArray<FName> Paths, const TArray<FName>& LoadBundles,TArray<FAsyncClassQuery> AdditionalClasses,bool Recursive);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static TArray<UClass*> GetDefaultClassesForAsync();
	

	TSet<FTopLevelAssetPath> Blueprints;
	static bool RemoveDuplicateClass(	UAssetManagerSettings* Settings, const UClass* Class);

	TArray<FAssetData> AssetData;
	TArray<FSoftObjectPath>ObjectsToLoad;
	UPROPERTY(BlueprintAssignable)
	FOnAsyncAssetLoaded Completed;

	TSharedPtr<FStreamableHandle> LoadHandle;
	 bool IsValidClass(FAssetData& Data, const FAsyncClassQuery& Class);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static FTopLevelAssetPath GetClassPath(UClass* Class);

	void BeginStream();
	void HandleLoadComplete();
	void HandlePIEEnded(bool Input);
};
