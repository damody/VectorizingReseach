#pragma once
#include "BasicMesh.h"

class UTriangulationCgal_SeedPoint;
class RBFMesh : public BasicMesh
{
public:
	BasicMesh::VertexIter vertices_begin()
	{
		return VertexIter(*this, VertexHandle(0));
	}

	//-----------------------------------------------------------------------------
	BasicMesh::VertexIter vertices_end()
	{
		return VertexIter(*this, VertexHandle(int32(n_vertices())));
	}

	RBFMesh(void);
	~RBFMesh(void);
	void ReadFromSeedpoint(UTriangulationCgal_SeedPoint* data, TArray<FVector> colors);
	FVector GetColor(float x, float y);
	FVector GetColor2Point(float x1, float y1, float x2, float y2);
	void lightblurC2();
	void blurC2();

};

