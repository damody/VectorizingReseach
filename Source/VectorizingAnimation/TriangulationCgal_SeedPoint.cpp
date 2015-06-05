#include "VectorizingAnimation.h"
#include "TriangulationCgal_SeedPoint.h"


void UTriangulationCgal_SeedPoint::AddPointIdx(float x, float y, int32 id)
{
	CgalTriangulation::Vertex_handle vh = TriangulationData.insert(Point(x, y));
	vh->info().mapid = id;
}

void UTriangulationCgal_SeedPoint::AddPoint(float x, float y)
{
	TriangulationData.insert(Point(x, y));
}

void UTriangulationCgal_SeedPoint::Compute()
{
	CTriDatas.Reset();
	FColorTriData t;
	Finite_faces_iterator fc = TriangulationData.finite_faces_begin();

	for (; fc != TriangulationData.finite_faces_end(); ++fc)
	{
		t.p[0] = FVector2D(fc->vertex(0)->point()[0], fc->vertex(0)->point()[1]);
		t.p[1] = FVector2D(fc->vertex(1)->point()[0], fc->vertex(1)->point()[1]);
		t.p[2] = FVector2D(fc->vertex(2)->point()[0], fc->vertex(2)->point()[1]);
		CTriDatas.Add(t);
	}
}

void UTriangulationCgal_SeedPoint::Clear()
{
	CTriDatas.Reset();
}

