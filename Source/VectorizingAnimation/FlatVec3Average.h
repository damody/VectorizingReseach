// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlatVec3Model.h"
#include "FlatVec3Average.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UFlatVec3Average : public UFlatVec3Model
{
	GENERATED_BODY()
public:

	virtual void BuildModel() override;

	UFUNCTION(BlueprintCallable, Category = "MathModel")
	FVector GetColor();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathModel")
	FVector ColorAverage;
};
