// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Object.h"
#include "EigenDef.h"
#include "LineV2.h"
#include "LineWidthFlatPolynomial.generated.h"



/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineWidthFlatPolynomial : public UObject
{
	GENERATED_BODY()
public:
	ULineWidthFlatPolynomial();

	UFUNCTION(BlueprintCallable, Category = "Line")
	void AddPoint(float x, float y, float w1, float w2);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	void ImportLine(const ULineV2* pos, const ULineV2* width);

	UFUNCTION(BlueprintCallable, Category = "Line")
	void Clear();

	UFUNCTION(BlueprintCallable, Category = "Line")
	FVector2D Interpolate(float dis);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	void BuildModel();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	float Length;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	TArray<float> Widths1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	TArray<float> Widths2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line")
	TArray<FVector2D> Pos;

	bool NeedBuildModel;

	VectorXd x1, x2;


};

