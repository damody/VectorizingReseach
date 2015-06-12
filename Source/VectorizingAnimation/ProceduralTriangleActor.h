// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProceduralMeshComponentX.h"
#include "ProceduralTriangleActor.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API AProceduralTriangleActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	// Allow viewing/changing the Material ot the procedural Mesh in editor (if placed in a level at construction)
	UPROPERTY(VisibleAnywhere, Category = Materials)
	UProceduralMeshComponentX* mesh;

	UPROPERTY(VisibleAnywhere, Category = Materials)
	FString MeshPath;

	UFUNCTION(BlueprintCallable, Category = "Procedural")
	void SetMaterial(UMaterialInterface* umi);

	UFUNCTION(BlueprintCallable, Category = "Procedural")
	void SetTrangles(TArray<FProceduralMeshTriangle> triangles);

	void GenerateTriangle(TArray<FProceduralMeshTriangle>& OutTriangles);
};
