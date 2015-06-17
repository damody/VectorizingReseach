// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ProceduralTriangleActor.h"
AProceduralTriangleActor::AProceduralTriangleActor(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	mesh = PCIP.CreateDefaultSubobject<UProceduralMeshComponentX>(this, TEXT("ProceduralTriangle"));
	// Apply a simple material directly using the VertexColor as its BaseColor input
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/BaseColor'"));
	mesh->SetMaterial(0, Material.Object);
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
	triangle.Vertex0.U = 0.0f;
	triangle.Vertex0.V = 0.0f;
	triangle.Vertex1.U = 1.0f;
	triangle.Vertex1.V = 0.0f;
	triangle.Vertex2.U = 0.5f;
	triangle.Vertex2.V = 0.75f;
	OutTriangles.Add(triangle);
}

void AProceduralTriangleActor::DrawLines(FVector center, TArray<ULineV2*> lines)
{
	for (int32 i = 0; i < lines.Num(); ++i)
	{
		ULineV2& nowline = *lines[i];
		for (int32 j = 0; j < nowline.Num() - 1; ++j)
		{
			FVector LinkStart = center, LinkEnd = center;
			LinkStart.X += nowline[j].X;
			LinkStart.Y += nowline[j].Y;
			LinkEnd.X += nowline[j + 1].X;
			LinkEnd.Y += nowline[j + 1].Y;
			DrawDebugLine(
				GetWorld(),
				LinkStart,
				LinkEnd,
				FColor(255, 0, 0),
				false, -1, 0,
				3.f
				);
		}
	}
}

void AProceduralTriangleActor::SetMaterial(UMaterialInterface* umi)
{
	mesh->SetMaterial(0, umi);
}

void AProceduralTriangleActor::SetTrangles(TArray<FProceduralMeshTriangle> triangles)
{
	mesh->SetProceduralMeshTriangles(triangles);
}


