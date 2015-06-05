// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "cvMat.h"
#include "DesktopPlatformModule.h"
#include "AssetHandlerActor.generated.h"

UCLASS()
class VECTORIZINGANIMATION_API AAssetHandlerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAssetHandlerActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "AssetHandler")
	static TArray<UcvMat*> GetWorkCVMat(FString path, FString outpath);

	UFUNCTION(BlueprintCallable, Category = "AssetHandler")
	static bool SaveObjectToAsset(UObject* data, FString outpath);

};
