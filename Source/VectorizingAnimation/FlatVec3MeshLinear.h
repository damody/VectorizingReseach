// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlatVec3Model.h"
#include "FlatVec3MeshLinear.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UFlatVec3MeshLinear : public UFlatVec3Model
{
	GENERATED_BODY()
public:

	virtual void BuildModel() override;

	UFUNCTION(BlueprintCallable, Category = "Line")
	FVector GetColor(float x, float y);

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line")
	FVector ColorAverage;
};
