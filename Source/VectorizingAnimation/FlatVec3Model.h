// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "FlatVec3Model.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class VECTORIZINGANIMATION_API UFlatVec3Model : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "AddPoint (float)"), Category = "Line")
	virtual void AddPoint(float x, float y, FVector c);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "AddPoint (Vector2D)"), Category = "Line")
	virtual void AddPoint_V2D(FVector2D pos, FVector c);
	
	virtual void BuildModel() PURE_VIRTUAL(UFlatVec3Model::BuildModel, return;);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line")
	bool NeedBuildModel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector> Colors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D> Pos;
};
