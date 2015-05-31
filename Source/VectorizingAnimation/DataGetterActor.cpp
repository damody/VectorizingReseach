// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "DataGetterActor.h"


// Sets default values
ADataGetterActor::ADataGetterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADataGetterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADataGetterActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

TArray<UcvMat*> ADataGetterActor::GetWorkCVMat(FString path)
{
	TArray<UcvMat*> res;
	TArray<UObject*> TextureAssets;
	EngineUtils::FindOrLoadAssetsByPath(path, TextureAssets, EngineUtils::ATL_Regular);
	for (int32 i = 0; i < TextureAssets.Num(); ++i)
	{
		UTexture2D* tex = Cast<UTexture2D>(TextureAssets[i]);
		if (tex != nullptr)
		{
			UcvMat* tmp = NewObject<UcvMat>();
			FString name;
			tex->GetName(name);
			name = TEXT("cvMat_") + name;
			tmp->Rename(*name);
			tmp->Texture = tex;
			tmp->UpdateTextureToCvMat();
			res.Add(tmp);
		}
	}
	return res;
}