// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "VectorizingData.h"
#include "Color.h"




TArray<FProceduralMeshTriangle> UVectorizingData::GenerateRenderData()
{
	TArray<FProceduralMeshTriangle> res;
	for (int32 i = 0; i < DrawTris.Num();++i)
	{
		FColorTriData& fctd = DrawTris[i];
		FProceduralMeshTriangle filler;
		filler.Vertex0.Position = FVector(0, fctd.p[0].X, fctd.p[0].Y);
		filler.Vertex0.Color = FColor((uint8)fctd.c[0].X, (uint8)fctd.c[0].Y, (uint8)fctd.c[0].Z);
		filler.Vertex1.Position = FVector(0, fctd.p[1].X, fctd.p[1].Y);
		filler.Vertex1.Color = FColor((uint8)fctd.c[1].X, (uint8)fctd.c[1].Y, (uint8)fctd.c[1].Z);
		filler.Vertex2.Position = FVector(0, fctd.p[2].X, fctd.p[2].Y);
		filler.Vertex2.Color = FColor((uint8)fctd.c[2].X, (uint8)fctd.c[2].Y, (uint8)fctd.c[2].Z);
		filler.Vertex0.U = 0.0f;
		filler.Vertex0.V = 0.0f;
		filler.Vertex1.U = 1.0f;
		filler.Vertex1.V = 0.0f;
		filler.Vertex2.U = 0.5f;
		filler.Vertex2.V = 0.75f;
		res.Add(filler);
	}
	return res;
}
