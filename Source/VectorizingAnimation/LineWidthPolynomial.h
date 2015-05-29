// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "EigenDef.h"
#include "LineV2.h"
#include "LineWidthPolynomial.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineWidthPolynomial : public UObject
{
	GENERATED_BODY()
public:
	ULineWidthPolynomial();

	UFUNCTION(BlueprintCallable, Category = "Line")
	void AddPoint(float x, float y, float w1, float w2);

	UFUNCTION(BlueprintCallable, Category = "Line")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "Line")
	FVector2D Interpolate(float dis);


	UFUNCTION(BlueprintCallable, Category = "Line")
	void BuildModel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Length;

	TArray<float> Widths1;

	TArray<float> Widths2;

	TArray<FVector2D> Pos;

	bool NeedBuildModel;

	VectorXd x1, x2;

};

