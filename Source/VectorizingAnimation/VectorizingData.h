// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TriangulationBase.h"
#include "ProceduralMeshComponentX.h"
#include "VectorizingData.generated.h"


/*
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UVectorizingData : public UObject
{  
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FColorTriData> DrawTris;

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	TArray<FProceduralMeshTriangle> GenerateRenderData();
	
	
};
