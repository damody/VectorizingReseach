// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ProceduralTriangleActor.h"
AProceduralTriangleActor::AProceduralTriangleActor(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("ProceduralTriangle"));
	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/BaseColor'"));
	//mesh->SetMaterial(0, Material.Object);
	//InternalReadMesh(MeshPath);
	TArray<FProceduralMeshTriangle> triangles;
	GenerateTriangle(triangles);
	mesh->SetProceduralMeshTriangles(triangles);
	RootComponent = mesh;
}

// Generate a single horizontal triangle counterclockwise to point up (one face, visible only from the top, not from the bottom)
void AProceduralTriangleActor::GenerateTriangle(TArray<FProceduralMeshTriangle>& OutTriangles)
{
	FProceduralMeshTriangle triangle;
	triangle.Vertex0.Position.Set(0.f, 0.f, 0.f);
	triangle.Vertex1.Position.Set(0.f, 640.f, 0.f);
	triangle.Vertex2.Position.Set(0.f, 640.f, 480.f);
	static const FColor Blue(51, 51, 255);
	triangle.Vertex0.Color = Blue;
	triangle.Vertex1.Color = Blue;
	triangle.Vertex2.Color = Blue;
	OutTriangles.Add(triangle);
	triangle.Vertex0.Position.Set(0.f, 0.f, 0.f);
	triangle.Vertex1.Position.Set(0.f, 640.f, 480.f);
	triangle.Vertex2.Position.Set(0.f, 0.f, 480.f);
	OutTriangles.Add(triangle);
}


void AProceduralTriangleActor::SetMaterial(UMaterialInterface* umi)
{
	mesh->SetMaterial(0, umi);
}

void AProceduralTriangleActor::SetTrangles(TArray<FProceduralMeshTriangle> triangles)
{
	mesh->SetProceduralMeshTriangles(triangles);
}


