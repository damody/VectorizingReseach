// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ueMat.generated.h"


UENUM(BlueprintType)
enum class EueMatEnum : uint8
{
	Error,
	UC_Gray,
	UC_BGR,
	FC_Gray,
	FC_BGR
};
/**
 * 
 */

UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UueMat : public UObject
{
	GENERATED_BODY()
public:
	
	operator cv::Mat()
	{
		return Pic;
	}
	
	cv::Mat Pic;

	void SetCvMat(cv::Mat input);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UueMat* UpdateCvMatToTexture();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	void UpdateTextureToCvMat();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UMaterialInterface* MakeMaterial();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	EueMatEnum GetMatState();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	void ConvertMat(EueMatEnum state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UMaterialInterface* MasterMaterialRef;
};
