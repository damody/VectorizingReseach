// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PicMesh.h"
#include "TriangulationBase.h"
#include "VectorizingData.h"
#include "IPicMesh.generated.h"

/**
 *  IPicMesh I is mean interface
 *	interface of picmesh
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UIPicMesh : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	static UIPicMesh* CreateIPicMeshInstance();

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void ReadFromSideline(UTriangulationCgal_Sideline* data);

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void SetSize(int32 w, int32 h);

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void MakeColor1();

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void MakeColor6(UcvMat* img);

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void MakeRegionLine(UcvMat* img, float lmax);

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void SetRegionColor(UcvMat* img);

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	void ComputeRegion();

	UFUNCTION(BlueprintPure, Category = "Vectorizing")
	TArray<ULineV2*> GetConstructLines();

	UFUNCTION(BlueprintCallable, Category = "Vectorizing")
	UVectorizingData* MakeVectorizingData();

	UFUNCTION(BlueprintPure, Category = "Vectorizing")
	TArray<FColorTriData>& GetTrangleData()
	{
		return mesh.m_Trangles;
	}

	PicMesh mesh;
};
