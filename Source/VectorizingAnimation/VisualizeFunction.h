// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "cvMat.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VisualizeFunction.generated.h"


/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UVisualizeFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OpenCV|Visualize")
	static void DrawLines(FVector center, TArray<ULineV2*> lines);
	
	UFUNCTION(BlueprintCallable, Category = "OpenCV|Visualize")
	static void ImShow(FString name, UcvMat* img);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Visualize")
	static void DestroyAllImShow();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Visualize")
	static void DestroyImShow(FString name);

};
