// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FlatVec3Model.h"
#include "RBFMesh.h"
#include "FlatVec3MeshLinear.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UFlatVec3MeshLinear : public UFlatVec3Model
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "MathModel")
	UFlatVec3MeshLinear* Clone();

	UFUNCTION(BlueprintCallable, Category = "MathModel")
	static TArray<UFlatVec3MeshLinear*> CloneArray(const TArray<UFlatVec3MeshLinear*>& src);

	virtual void BuildModel() override;

	UFUNCTION(BlueprintCallable, Category = "MathModel")
	FVector GetColor(float x, float y);

	UFUNCTION(BlueprintCallable, Category = "MathModel")
	FVector GetColor2Point(float x1, float y1, float x2, float y2);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MathModel")
	FVector ColorAverage;


	RBFMesh	mesh;
};
