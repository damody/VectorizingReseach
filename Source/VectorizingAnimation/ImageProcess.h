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
	static UueMat*	WhiteBalance(UueMat* umat);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UueMat*	ImgSharpen(UueMat* umat);
	
	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UueMat*	PowerEnhance(UueMat* umat, double power);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UueMat*	NormalizeUC(UueMat* umat, double vmin, double vmax);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UueMat*	NormalizeFC(UueMat* umat, double vmin, double vmax);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
	static UueMat*	BilateralFilter(UueMat* umat, int32 maskSize);
};
