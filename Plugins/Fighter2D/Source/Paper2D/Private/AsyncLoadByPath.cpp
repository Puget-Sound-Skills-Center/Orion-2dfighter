// Fill out your copyright notice in the Description page of Project Settings.



#include "AsyncLoadByPath.h"
#include "ModuleDescriptor.h"

#include "PaperRuntimeSettings.h"
#include "Async/Async.h"
#include "Engine/AssetManager.h"
#include "Engine/AssetManagerSettings.h"
#include "Engine/Blueprint.h"
#include "Misc/DateTime.h"
#if WITH_EDITOR
#include "Editor.h"
#endif
class UPaperRuntimeSettings;

void UAsyncLoadByPath::Activate()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
		"AssetRegistry");

	IAssetRegistry* AssetRegistry = &IAssetRegistry::GetChecked();
	AssetRegistry->GetAssetsByPaths(Paths, AssetData, Recursive);
	UE_LOG(LogTemp,Log,TEXT("Begin Async Loading"));
	TArray<FTopLevelAssetPath> BlueprintClasses;
	for (const auto Class : AdditionalClasses)
	{
		if (Class.Class)
		{
			if (Class.HasBlueprints)
			{
				BlueprintClasses.Add(Class.Class->GetClassPathName());
			}
		}
	}
	if (!BlueprintClasses.IsEmpty())
	{
		AssetRegistry->GetDerivedClassNames(BlueprintClasses, {}, Blueprints);
	}

	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [&]()
	{
		for (auto Asset : AssetData)
		{
			for (const auto Class : AdditionalClasses)
			{
				if (IsValidClass(Asset, Class))
				{
					ObjectsToLoad.Add(Asset.GetSoftObjectPath());
					break;
				}
			}
		}


		UE_LOG(LogTemp,Log,TEXT("Begin Async Stream"));
		AsyncTask(ENamedThreads::GameThread, [&]() { BeginStream(); });
	});
}


UAsyncLoadByPath* UAsyncLoadByPath::AsyncLoadByPath(UObject* worldContextObject, TArray<FName> Paths,
                                                    const TArray<FName>& LoadBundles,
                                                    TArray<FAsyncClassQuery> AdditionalClasses, bool Recursive)
{
	UAsyncLoadByPath* Action = NewObject<UAsyncLoadByPath>();
	Action->BeginTime = FDateTime::Now();
	Action->Paths = Paths;
	Action->Recursive = Recursive;
	Action->AdditionalClasses = AdditionalClasses;
	Action->RegisterWithGameInstance(worldContextObject);
	return Action;
}

TArray<UClass*> UAsyncLoadByPath::GetDefaultClassesForAsync()
{
	UPaperRuntimeSettings* PaperRuntimeSettings = GetMutableDefault<UPaperRuntimeSettings>();
	return PaperRuntimeSettings->AsyncLoadingClasses;
}


bool UAsyncLoadByPath::RemoveDuplicateClass(UAssetManagerSettings* Settings, const UClass* Class)
{
#if WITH_EDITOR
	for (int Index = 0; Index != Settings->PrimaryAssetTypesToScan.Num(); ++Index)
	{
		auto Scan = Settings->PrimaryAssetTypesToScan[Index];
		if (Scan.GetAssetBaseClass()->GetFName() == Class->GetFName())
		{
			Settings->PrimaryAssetTypesToScan.RemoveAt(Index);
			return true;
		}
	}
#endif

	return false;
}


bool UAsyncLoadByPath::IsValidClass(FAssetData& Data, const FAsyncClassQuery& Class)
{
	if (Class.Class && Data.GetClass())
	{
		if (Data.GetClass()->IsChildOf(Class.Class) || Data.GetClass() == Class.Class)
		{
			return true;
		}

		else if (Data.GetClass() == UBlueprint::StaticClass())
		{
			for (auto BP : Blueprints)
			{
				if (BP.GetAssetName().ToString() == Data.AssetName.ToString() + "_C")
				{
					return true;
				}
			}
		}
	}
	return false;
}

FTopLevelAssetPath UAsyncLoadByPath::GetClassPath(UClass* Class)
{
	return Class->GetClassPathName();
}

void UAsyncLoadByPath::BeginStream()
{
	UE_LOG(LogTemp,Log,TEXT("Begin Stream "));
	if (this)
	{
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		LoadHandle = Streamable.RequestAsyncLoad(ObjectsToLoad, FStreamableDelegate(),
		                                         FStreamableManager::AsyncLoadHighPriority);
		if (LoadHandle.IsValid())
		{
			if (!LoadHandle->HasLoadCompleted())
			{
				LoadHandle->BindCompleteDelegate(FStreamableDelegate::CreateUObject(this, &UAsyncLoadByPath::HandleLoadComplete));
#if WITH_EDITOR
				FEditorDelegates::PrePIEEnded.AddUObject(this,&UAsyncLoadByPath::HandlePIEEnded);
				
#endif
			}else
			{
				HandleLoadComplete();
			}
		}else
		{
			HandleLoadComplete();
		}
	}
}

void UAsyncLoadByPath::HandleLoadComplete()
{
	//can be invalid if play in editor ended
	if (this)
	{
		TArray<UObject*> AssetsLoaded;
		if (LoadHandle.IsValid())
		{
			LoadHandle->GetLoadedAssets(AssetsLoaded);
		
			UE_LOG(LogTemp, Log, TEXT("Async loaded %d Objects in %f miliseconds"), AssetsLoaded.Num(),
				   (FDateTime::Now()-BeginTime).GetTotalMilliseconds())
		}
		Completed.Broadcast(AssetsLoaded);
		SetReadyToDestroy();
		LoadHandle.Reset();
	}
}

void UAsyncLoadByPath::HandlePIEEnded(bool Input)
{
	if (this)
	{
		if (LoadHandle)
		{
			if (!LoadHandle->HasLoadCompleted())
			{
				LoadHandle->CancelHandle();
				SetReadyToDestroy();
				Completed.Broadcast({});
				LoadHandle.Reset();
				UE_LOG(LogTemp,Warning,TEXT("PIE ended in middle of async loading canceling"));
				
			}
		}
	}
#if WITH_EDITOR
	FEditorDelegates::PrePIEEnded.RemoveAll(this);
#endif
	
}
