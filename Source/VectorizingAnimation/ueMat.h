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
	
	UueMat(){}

	UueMat(cv::Mat mat) : pic(mat){}

	operator cv::Mat()
	{
		return pic;
	}
	
	cv::Mat pic;
	
	void Copy(cv::Mat input);
	// fir c++ function
	static float ColorDistance(cv::Mat& a, cv::Mat& b);
	static void ImgFillBlack(cv::Mat& a, cv::Mat& b);

	// Blueprint function

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UueMat* Clone();

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

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	static TArray<UueMat*> MakeStaticBackGroundByMove(const TArray<UueMat*>& m_Video, 
			TArray<FVector2D>& m_Moves, UueMat* background, UueMat* foreground);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UMaterialInterface* MasterMaterialRef;
};
