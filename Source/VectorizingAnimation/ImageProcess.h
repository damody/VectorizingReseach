// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlatVec3MeshLinear.h"
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
    static UcvMat*  WhiteBalance(UcvMat* umat);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat*  ImgSharpen(UcvMat* umat);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat*  PowerEnhance(UcvMat* umat, float power);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat*  NormalizeUC(UcvMat* umat, float vmin, float vmax);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat*  NormalizeFC(UcvMat* umat, float vmin, float vmax);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat*  BilateralFilter(UcvMat* umat, int32 maskSize);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat* DrawWhileLine(UcvMat* umat, const TArray<ULineV2*> draw);

    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static UcvMat* FixSpaceLineX(UcvMat* umat, UcvMat* oriImg, float initdis);



    UFUNCTION(BlueprintCallable, Category = "OpenCV|ImageProcess")
    static TArray<UFlatVec3MeshLinear*> BuildColorModelV8(UcvMat* image, UcvMat* oimg, UcvMat* mask2);
};

FORCEINLINE FVector Vec3bToVector(const cv::Vec3b c)
{
    return FVector(c[0], c[1], c[2]);
}
