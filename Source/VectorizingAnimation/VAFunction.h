// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VAFunction.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UVAFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Flie")
	static bool OpenFile(FString& path);
		
	UFUNCTION(BlueprintCallable, Category = "Nop")
	static void Nop();

	UFUNCTION(BlueprintCallable, Category = "OpenCV")
	static FVector2D GetMoveVectorBySURF(UcvMat* prevgray, UcvMat* gray);
};
