// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GeneralDef.h"
#include "cvMat.generated.h"


UENUM(BlueprintType)
enum class EcvMatEnum : uint8
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
class VECTORIZINGANIMATION_API UcvMat : public UObject
{
	GENERATED_BODY()
public:
	
	UcvMat(){}

	UcvMat(cv::Mat mat) : pic(mat){}

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
	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	int32 Width();

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	int32 Height();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UcvMat* Clone();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UcvMat* UpdateCvMatToTexture();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	void UpdateTextureToCvMat();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UMaterialInterface* MakeMaterial();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	EcvMatEnum GetMatState();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	UcvMat* ConvertMat(EcvMatEnum state);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	static TArray<UcvMat*> MakeStaticBackGroundByMove(const TArray<UcvMat*>& m_Video, 
			TArray<FVector2D>& m_Moves, UcvMat* background, UcvMat* foreground);

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	FVector GetBilinearColor(float x, float y) const;

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	float GetBilinearLight(float x, float y) const;

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	float GetLight(int32 x, int32 y) const;

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	FVector GetColor(int32 x, int32 y) const;

	UFUNCTION(BlueprintPure, Category = "OpenCV|Mat")
	int32 GetIndex(int32 x, int32 y) const;

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Mat")
	TArray<float> GetLineLight(float x1, float y1, float x2, float y2, int32 div);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	UTexture2D* Texture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mat")
	UMaterialInterface* MasterMaterialRef;
};
