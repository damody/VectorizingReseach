// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "AssetHandlerActor.h"
#include "AssetRegistryModule.h"
#include "Misc/OutputDevice.h"
#include "FileHelpers.h"



// Sets default values
AAssetHandlerActor::AAssetHandlerActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAssetHandlerActor::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AAssetHandlerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

TArray<UcvMat*> AAssetHandlerActor::GetWorkCVMat(FString path, FString outpath)
{
    TArray<UcvMat*> res;
    TArray<UObject*> TextureAssets;
    EngineUtils::FindOrLoadAssetsByPath(path, TextureAssets, EngineUtils::ATL_Regular);
    for(int32 i = 0; i < TextureAssets.Num(); ++i)
    {
        UTexture2D* tex = Cast<UTexture2D>(TextureAssets[i]);
        if(tex != nullptr)
        {
            // make tmp asset
            if(outpath.Len() > 4)
            {
                FString name;
                tex->GetName(name);
                FString PackageName = outpath;
                PackageName += name;
                UPackage* Package = CreatePackage(NULL, *PackageName);

				UcvMat* NewAsset = NewObject<UcvMat>(Package, FName("CvMat"), RF_Public);
                if(NewAsset != NULL)
                {
                    // Fill in the assets data here
                    //UcvMat* tmp = NewObject<UcvMat>();
                    UcvMat* tmp = NewAsset;
                    name = TEXT("cvMat_") + name;
                    tmp->Rename(*name);
                    tmp->Texture = tex;
                    tmp->UpdateTextureToCvMat();
                    res.Add(tmp);
                    FAssetRegistryModule::AssetCreated(NewAsset);
                }
            }
            else
            {
                FString name;
                tex->GetName(name);
                // just make dynamic object
                UcvMat* tmp = NewObject<UcvMat>();
                name = TEXT("cvMat_") + name;
                tmp->Rename(*name);
                tmp->Texture = tex;
                tmp->UpdateTextureToCvMat();
                res.Add(tmp);
            }
        }
    }
    return res;
}

bool AAssetHandlerActor::SaveObjectToAsset(UObject* data, FString outpath)
{
    FString name;
    data->GetName(name);
    FString PackageName = outpath;
    PackageName += name;
    UPackage* Package = CreatePackage(NULL, *PackageName);
	UObject* NewAsset = DuplicateObject(data, Package);
	NewAsset->Rename(*name);
	FAssetRegistryModule::AssetCreated(NewAsset);
	NewAsset->MarkPackageDirty();
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
    return UPackage::SavePackage(Package, data, RF_Public, *PackageFileName, GError, nullptr, false, true, SAVE_None);
}
