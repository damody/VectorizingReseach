
#include "VectorizingAnimation.h"
#include "TriangulationCgal_Sideline.h"

void UTriangulationCgal_Sideline::AddPoint(double x, double y)
{
    m_SeedPoints.push_back(Point(x, y));
}

int UTriangulationCgal_Sideline::Compute()
{
    m_Triangulation.clear();
    Vertex_handles vhs;
    // add 4 conner point
    Point lu(0, 0);
    Point ld(0, m_h);
    Point ru(m_w, m_h);
    Point rd(m_w, 0);
    CTriangulation::Vertex_handle v1 = m_Triangulation.insert(lu);
    CTriangulation::Vertex_handle v2 = m_Triangulation.insert(ld);
    CTriangulation::Vertex_handle v3 = m_Triangulation.insert(ru);
    CTriangulation::Vertex_handle v4 = m_Triangulation.insert(rd);
    v1->info().nesting_level = 0;
    v2->info().nesting_level = 0;
    v3->info().nesting_level = 0;
    v4->info().nesting_level = 0;
//     m_Triangulation.insert_constraint(v1, v2);
//     m_Triangulation.insert_constraint(v2, v3);
//     m_Triangulation.insert_constraint(v3, v4);
//     m_Triangulation.insert_constraint(v4, v1);
    Insert_lines(m_Triangulation);
    Mesher mesher(m_Triangulation);
    mesher.set_criteria(m_Criteria);
    //mesher.mark_facets();
    mesher.refine_mesh();
    mark_domains2(m_Triangulation);
	for (int i = 0; i < m_Lines.Num(); ++i)
    {
        ULineV2& line = *m_Lines[i];
        for(int j = 0; j < line.Num(); ++j)
        {
            Point now(line[j].X, line[j].Y);
            CTriangulation::Vertex_handle vh = m_Triangulation.nearest_vertex(now);
        }
    }
    
    return 0;
}

void UTriangulationCgal_Sideline::SetCriteria(float shapebound, float length)
{
    m_Criteria = Criteria(shapebound, length);
}

void UTriangulationCgal_Sideline::Clear()
{
    m_SeedPoints.clear();
}

void UTriangulationCgal_Sideline::Insert_lines(CTriangulation& cdt)
{
    Point last;
	for (int i = 0; i < m_Lines.Num(); ++i)
    {
        ULineV2& line = *m_Lines[i];
        last = Point(line[0].X, line[0].Y);
        Point start = last;
        CTriangulation::Vertex_handle v_prev  = cdt.insert(last);
        for(int j = 0; j < line.Num(); ++j)
        {
            Point now(line[j].X, line[j].Y);
            if(now != last)
            {
                CTriangulation::Vertex_handle vh = m_Triangulation.insert(now);
                vh->info().nesting_level = i;
                m_Triangulation.insert_constraint(v_prev, vh);
                v_prev = vh;
                last = now;
            }
        }
    }
}

void UTriangulationCgal_Sideline::SetSize(int w, int h)
{
    m_w = w;
    m_h = h;
}

void    UTriangulationCgal_Sideline::mark_domains2(CTriangulation& ct, CTriangulation::Face_handle start, int index,
        std::list<CTriangulation::Edge>& border, int color_label)
{
    if(start->info().nesting_level != TRIANGLE_NOT_INIT)
    {
        return;
    }
    std::list<CTriangulation::Face_handle> queue;
    queue.push_back(start);
    while(! queue.empty())
    {
        CTriangulation::Face_handle fh = queue.front();
        queue.pop_front();
        if(fh->info().nesting_level == TRIANGLE_NOT_INIT)
        {
            fh->info().nesting_level = index;
            fh->info().color_label = color_label;
            for(int i = 0; i < 3; i++)
            {
                CTriangulation::Edge e(fh, i);
                CTriangulation::Face_handle n = fh->neighbor(i);
                if(n->info().nesting_level == TRIANGLE_NOT_INIT)
                {
                    if(ct.is_constrained(e))
                    {
                        border.push_back(e);
                    }
                    else
                    {
                        queue.push_back(n);
                    }
                }
            }
        }
    }
}

int UTriangulationCgal_Sideline::mark_domains2(CTriangulation& cdt)
{
    for(CTriangulation::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it)
    {
        it->info().nesting_level = TRIANGLE_NOT_INIT;
    }
    int cc = 0;
    int index = 1;
    std::list<CTriangulation::Edge> border;
    Finite_faces_iterator fc = m_Triangulation.finite_faces_begin();
    for(; fc != m_Triangulation.finite_faces_end(); ++fc)
    {
        if(TRIANGLE_NOT_INIT == fc->info().nesting_level)
        {
            mark_domains2(cdt, fc, index++, border, cc++);
            break;
        }
    }
    while(! border.empty())
    {
        CTriangulation::Edge e = border.front();
        border.pop_front();
        CTriangulation::Face_handle n = e.first->neighbor(e.second);
        if(n->info().nesting_level == TRIANGLE_NOT_INIT)
        {
            mark_domains2(cdt, n, e.first->info().nesting_level + 1, border, cc++);
        }
    }
    m_RegionCount = cc;
    printf("m_RegionCount %d\n", m_RegionCount);
    return cc;
}


void UTriangulationCgal_Sideline::MakeColorSequential()
{
    m_Triangles.Reset();
	FColorTriData t;
    Finite_faces_iterator fc = m_Triangulation.finite_faces_begin();
    for(; fc != m_Triangulation.finite_faces_end(); ++fc)
    {
        if(fc->is_in_domain() && fc->info().in_domain()
                && fc->info().nesting_level != TRIANGLE_TRANSPARENT)
        {
            t.p[0] = FVector2D(fc->vertex(0)->point()[0], fc->vertex(0)->point()[1]);
            t.p[1] = FVector2D(fc->vertex(1)->point()[0], fc->vertex(1)->point()[1]);
            t.p[2] = FVector2D(fc->vertex(2)->point()[0], fc->vertex(2)->point()[1]);
            int label = fc->info().color_label;
            FVector vm;
            vm.X = label % 255;
            vm.Y = (label / 255) % 255;
            vm.Z = label / 255 / 255;
			t.c[0] = vm;
			t.c[1] = vm;
			t.c[2] = vm;
            m_Triangles.Add(t);
        }
    }
}

void UTriangulationCgal_Sideline::AddLines(const TArray<ULineV2*>& is)
{
    m_Lines.Append(is);
}

void UTriangulationCgal_Sideline::SetColor(const TArray<UFlatVec3MeshLinear*>& colors)
{
	m_ColorConstraint = UFlatVec3MeshLinear::CloneArray(colors);
}

void UTriangulationCgal_Sideline::MakeColorByModel()
{
	FColorTriData t;
	m_Triangles.Reset();
	Finite_faces_iterator fc;
	fc = m_Triangulation.finite_faces_begin();
	for(; fc != m_Triangulation.finite_faces_end(); ++fc)
	{
		if(fc->is_in_domain())
		{
			t.p[0] = FVector2D(fc->vertex(0)->point()[0], fc->vertex(0)->point()[1]);
			t.p[1] = FVector2D(fc->vertex(1)->point()[0], fc->vertex(1)->point()[1]);
			t.p[2] = FVector2D(fc->vertex(2)->point()[0], fc->vertex(2)->point()[1]);
			int idx = fc->info().color_label;
			if(idx >= 0 && idx < m_ColorConstraint.Num())
			{
				t.c[0] = m_ColorConstraint[idx]->GetColor(t.p[0].X, t.p[0].Y);
				t.c[1] = m_ColorConstraint[idx]->GetColor(t.p[1].X, t.p[1].Y);
				t.c[2] = m_ColorConstraint[idx]->GetColor(t.p[2].X, t.p[2].Y);
			}
			m_Triangles.Add(t);
		}
	}
}
