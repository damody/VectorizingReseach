// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImageProcess.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UImageProcess : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	WhiteBalance(UcvMat* umat);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	ImgSharpen(UcvMat* umat);
	
	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	PowerEnhance(UcvMat* umat, float power);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	NormalizeUC(UcvMat* umat, float vmin, float vmax);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	NormalizeFC(UcvMat* umat, float vmin, float vmax);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UcvMat*	BilateralFilter(UcvMat* umat, int32 maskSize);
};
