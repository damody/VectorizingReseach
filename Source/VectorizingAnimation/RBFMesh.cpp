#include "VectorizingAnimation.h"
#include "RBFMesh.h"
#include "TriangulationCgal_SeedPoint.h"
#include "SortFunction.h"

RBFMesh::RBFMesh(void)
{
    request_vertex_status();
    request_edge_status();
    request_face_status();
}


RBFMesh::~RBFMesh(void)
{
    release_vertex_status();
    release_edge_status();
    release_face_status();
}

void RBFMesh::ReadFromSeedpoint(UTriangulationCgal_SeedPoint* ts, TArray<FVector> colors)
{
    UTriangulationCgal_SeedPoint::Finite_vertices_iterator vc;
    UTriangulationCgal_SeedPoint::Finite_faces_iterator fc;
    vc = ts->TriangulationData.finite_vertices_begin();
    int i = 0;
    for(; vc != ts->TriangulationData.finite_vertices_end(); ++vc)
    {
        BasicMesh::Point p(vc->point()[0], vc->point()[1], 0);
        VertexHandle v = add_vertex(p);
        vc->info().idx = i;
        i++;
        data(v).c = colors[vc->info().mapid];
    }
    fc = ts->TriangulationData.finite_faces_begin();
    for(; fc != ts->TriangulationData.finite_faces_end(); ++fc)
    {
        BasicMesh::VHandle vh[3];
        vh[0] = BasicMesh::VHandle(fc->vertex(0)->info().idx);
        vh[1] = BasicMesh::VHandle(fc->vertex(1)->info().idx);
        vh[2] = BasicMesh::VHandle(fc->vertex(2)->info().idx);
        FaceHandle f = add_face(vh[0], vh[1], vh[2]);
    }
}

FVector RBFMesh::GetColor(float x, float y)
{
    BasicMesh::Point now(x, y, 0);
    BasicMesh::VHandle findvh = *(vertices_begin());
    float mindis = (point(findvh) - now).sqrnorm();
    int minidx = 0;
    int ii = 0;

    for(ii = 0; minidx >= 0; ++ii)
    {
        BasicMesh::Point lastp = point(findvh);
        minidx = -1;
        for(VVIter vvit = vv_iter(findvh); vvit.is_valid(); ++vvit, ii++)
        {
            Point tp = point(*vvit);
            FVector2D p(tp[0], tp[1]);
            float dis = (tp - now).sqrnorm();
            if(dis < mindis)
            {
                mindis = dis;
                minidx = ii;
                findvh = *vvit;
            }
        }
    }
    if(mindis < 0.1)
    {
        return data(findvh).c;
    }
    return data(findvh).c;
//  printf("mindis %f\n", mindis);
    //return Vector3(0, 0, 255);
    TArray<float> diss;
    float sum = 0;
    for(VVIter vvit = vv_iter(findvh); vvit.is_valid(); ++vvit)
    {
        if(data(*vvit).c != FVector())
        {
            float dis = (point(*vvit) - now).norm();
            dis = 1 / dis;
            diss.Add(dis);
            sum += dis;
        }
    }
    sum = 1 / sum;
    for(int i = 0; i < diss.Num(); ++i)
    {
        diss[i] *= sum;
    }
    FVector res;
    ii = 0;
    for(VVIter vvit = vv_iter(findvh); vvit.is_valid(); ++vvit)
    {
        if(data(*vvit).c != FVector())
        {
            res += diss[ii++] * data(*vvit).c;
        }
    }
    return res;
}

FVector RBFMesh::GetColor(float x1, float y1, float x2, float y2)
{
    BasicMesh::Point now((x1 + x2 * 2) * 0.333, (y1 + y2 * 2) * 0.333, 0);
    BasicMesh::VHandle findvh = *(vertices_begin());
    float mindis = (point(findvh) - now).sqrnorm();
    int minidx = 0;
    int ii = 0;

    for(ii = 0; minidx >= 0; ++ii)
    {
        BasicMesh::Point lastp = point(findvh);
        minidx = -1;
        for(VVIter vvit = vv_iter(findvh); vvit.is_valid(); ++vvit, ii++)
        {
            Point tp = point(*vvit);
            FVector2D p(tp[0], tp[1]);
            float dis = (tp - now).sqrnorm();
            if(dis < mindis)
            {
                mindis = dis;
                minidx = ii;
                findvh = *vvit;
            }
        }
    }
    return data(findvh).c;
}

void RBFMesh::lightblurC2()
{
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            int count = 1;
            FVector blur = data(*vit).c;
            for(VVIter vv_itr = vv_iter(*vit) ; vv_itr.is_valid(); ++vv_itr)
            {
                if(data(*vv_itr).constraint == 0)
                {
                    count++;
                    blur += data(*vv_itr).c;
                }
            }
            blur /= count;
            float light = GetLight(blur);
            count = 1;
            blur = data(*vit).c;
            for(VVIter vv_itr = vv_iter(*vit) ; vv_itr.is_valid(); ++vv_itr)
            {
                if(data(*vv_itr).constraint == 0)
                {
                    if(GetLight(data(*vv_itr).c) > light)
                    {
                        count += 5;
                        blur += data(*vv_itr).c * 5;
                    }
                    else
                    {
                        count++;
                        blur += data(*vv_itr).c;
                    }
                }
            }
            blur /= count;
            data(*vit).c3 = blur;
        }
    }
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            data(*vit).c = data(*vit).c3;
        }
    }
}

void RBFMesh::blurC2()
{
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            int count = 1;
            FVector blur = data(*vit).c2;
            for(VVIter vv_itr = vv_iter(*vit) ; vv_itr.is_valid(); ++vv_itr)
            {
                if(data(*vv_itr).constraint == 0)
                {
                    count++;
                    blur += data(*vv_itr).c2;
                }
            }
            blur /= count;
            data(*vit).c3 = blur;
        }
    }
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            data(*vit).c2 = data(*vit).c3;
        }
    }
}
