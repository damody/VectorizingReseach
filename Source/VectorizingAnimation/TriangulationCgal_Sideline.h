#pragma once
#include "TriangulationBase.h"
#pragma warning( push, 0 )
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/Color.h>
#include <CGAL/Triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/Polygon_2.h>
#pragma warning( pop )

#include "LineDef.h"
#include "IntsDuplex.h"
#include "FlatVec3MeshLinear.h"
#include "TriangulationCgal_Sideline.generated.h"


UCLASS(BlueprintType)
class UTriangulationCgal_Sideline : public UTriangulationBase
{
	GENERATED_BODY()
public:
    struct FaceInfo2
    {
        FaceInfo2(): nesting_level(-1), color_label(0) {}
        int nesting_level;
        int color_label;
        bool in_domain()
        {
            return nesting_level >= 0;
        }
        bool in_domain2()
        {
            return nesting_level % 2 == 1;
        }
    };
    
    struct VertexInfo2
    {
        VertexInfo2(): nesting_level(-1) {}
		int nesting_level;
		int idx;
    };
    typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
    //typedef CGAL::Triangulation_vertex_base_2<K>                    Vb;
    typedef CGAL::Triangulation_vertex_base_with_info_2<VertexInfo2, K> Vb;
    typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K> Fbbb;
    typedef CGAL::Constrained_triangulation_face_base_2<K, Fbbb>    Fbb;
    typedef CGAL::Delaunay_mesh_face_base_2<K, Fbb>         Fb;
    typedef CGAL::Triangulation_data_structure_2<Vb, Fb>            TDS;
    typedef CGAL::Exact_predicates_tag                              Itag;
    typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CTriangulation;
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> Triangulation;
    typedef CGAL::Delaunay_mesh_size_criteria_2<CTriangulation>  Criteria;
    typedef CGAL::Delaunay_mesher_2<CTriangulation, Criteria>    Mesher;
    typedef CGAL::Polygon_2<K>                  Polygon;
    
    typedef CTriangulation::Face_handle          Face_handle;
    typedef CTriangulation::Finite_faces_iterator        Finite_faces_iterator;
	typedef CTriangulation::Finite_edges_iterator        Finite_edges_iterator;
    typedef CTriangulation::Finite_vertices_iterator     Finite_vertices_iterator;
    typedef CTriangulation::Vertex_handle            Vertex_handle;
    typedef CTriangulation::Point                Point;
    
    typedef std::vector<Point>      Points;
    typedef std::vector<Vertex_handle>  Vertex_handles;
    
    void    AddLines(const TArray<ULineV2*>& is);
	
    virtual void AddPoint(double x, double y);
    virtual int Compute();
	void	MakeColorByModel();
    virtual void Clear();
    void    MakeColorSequential();
    void    MakeColorRandom();
    void    SetSize(int w, int h);
    void    SetCriteria(float shapebound, float length);
       
    void mark_domains2(CTriangulation& ct, CTriangulation::Face_handle start,
                       int index, std::list<CTriangulation::Edge>& border, int color_label);
    int mark_domains2(CTriangulation& cdt);

	void SetColor(const TArray<UFlatVec3MeshLinear*>& colors);

    void Insert_lines(CTriangulation& cdt);
    static const int TRIANGLE_NOT_INIT = -1;
    static const int TRIANGLE_TRANSPARENT = -2;
    
    int     m_RegionCount;
    CTriangulation   m_Triangulation;
    Criteria    m_Criteria;
    Points      m_SeedPoints;
    Polygon     m_Polygon;
    int     m_w, m_h;
	TArray<UFlatVec3MeshLinear*>   m_ColorConstraint;
	TArray<ULineV2*>       m_Lines;
    TArray<ULineV2Duplex*> m_LinesWidth;
	TArray<FColorTriData>	m_Triangles;
	TArray<UIntsDuplex*>	m_i2s;
};


