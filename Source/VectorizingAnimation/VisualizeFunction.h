// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VisualizeFunction.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UVisualizeFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "OpenCV|Visualize")
	static void DrawLines(TArray<UueLine*> lines);
	
	
};
