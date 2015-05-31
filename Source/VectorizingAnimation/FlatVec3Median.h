// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlatVec3Model.h"
#include "FlatVec3Median.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UFlatVec3Median : public UFlatVec3Model
{
	GENERATED_BODY()
public:
	
	virtual void BuildModel() override;

	UFUNCTION(BlueprintCallable, Category = "MathModel")
	FVector GetColor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathModel")
	FVector ColorMedian;
	
};
