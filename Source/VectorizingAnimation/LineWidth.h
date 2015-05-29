// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "LineV2.h"
#include "LineWidth.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineWidth : public UObject
{
	GENERATED_BODY()
public:
	ULineWidth();

	UFUNCTION(BlueprintCallable, Category = "Line")
	void AddPoint(float x, float y, float w1, float w2);

	UFUNCTION(BlueprintCallable, Category = "Line")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "Line")
	void ImportLine(const ULineV2* pos, const ULineV2* width);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Length;

	TArray<float> Widths1;
	
	TArray<float> Widths2;
	
	TArray<FVector2D> Pos;

};

