#pragma once

#pragma warning(disable:4503)
#include "TriangulationBase.h"
#pragma warning( push, 0 )
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/Polygon_2.h>
#pragma warning( pop ) 
#include "TriangulationCgal_SeedPoint.generated.h"

UCLASS(BlueprintType)
class UTriangulationCgal_SeedPoint : public UTriangulationBase
{
	GENERATED_BODY()
public:
	struct FaceInfo2
	{
	};
	struct VertexInfo2
	{
		VertexInfo2():idx(-1),mapid(-1){}
		int32 idx;
		int32 mapid;
	};
	typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
	typedef CGAL::Triangulation_vertex_base_with_info_2<VertexInfo2, K> Vb;
	//typedef CGAL::Triangulation_vertex_base_2<K> Vb;
	typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K> Fbbb;
	typedef CGAL::Constrained_triangulation_face_base_2<K, Fbbb>    Fbb;
	typedef CGAL::Delaunay_mesh_face_base_2<K, Fbb>         Fb;
	//typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CgalTriangulation;

	typedef CgalTriangulation::Face_handle Face_handle;
	typedef CgalTriangulation::Finite_faces_iterator Finite_faces_iterator;
	typedef CgalTriangulation::Finite_vertices_iterator Finite_vertices_iterator;
	typedef CgalTriangulation::Point  Point;

	virtual ~UTriangulationCgal_SeedPoint() {}
	void AddPointIdx(float x, float y, int32 id);
	virtual void AddPoint(float x, float y);
	virtual void Compute();
	virtual void Clear();

	CgalTriangulation   TriangulationData;
};

