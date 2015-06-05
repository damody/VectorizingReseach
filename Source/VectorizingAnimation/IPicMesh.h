// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "PicMesh.h"
#include "IPicMesh.generated.h"

/**
 *  IPicMesh I is mean interface
 *	interface of picmesh
 */
UCLASS()
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

	PicMesh mesh;
};