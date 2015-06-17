#include "VectorizingAnimation.h"
#include <algorithm>
#include "PicMesh.h"
#include "TriangulationCgal_Sideline.h"
#include "cvMat.h"
#include "LineV2.h"
#include "GeneralDef.h"
#include "SortFunction.h"


PicMesh::PicMesh(void)
{
    request_vertex_status();
    request_edge_status();
    request_face_status();
}


PicMesh::~PicMesh(void)
{
    release_vertex_status();
    release_edge_status();
    release_face_status();
}

void PicMesh::ReadFromSideline(UTriangulationCgal_Sideline* ts)
{
	if (ts->DuplexIndexs.Num() == 0)
	{
		return;
	}
	m_Lines = ULineV2::CloneArray(ts->LinesPos);
	m_i2s = UIntsDuplex::CloneArray(ts->DuplexIndexs);
	m_ColorConstraint = UFlatVec3MeshLinear::CloneArray(ts->ColorModels);
    UTriangulationCgal_Sideline::Finite_vertices_iterator vc;
    UTriangulationCgal_Sideline::Finite_faces_iterator fc;
    vc = ts->m_Triangulation.finite_vertices_begin();
    int32 i = 0;
    for(; vc != ts->m_Triangulation.finite_vertices_end(); ++vc)
    {
        BasicMesh::Point p(vc->point()[0], vc->point()[1], 0);
        VertexHandle v = add_vertex(p);
        vc->info().idx = i;
        i++;
    }
    int32 maxregion = 0;
    fc = ts->m_Triangulation.finite_faces_begin();
    for(; fc != ts->m_Triangulation.finite_faces_end(); ++fc)
    {
        if(fc->is_in_domain())
        {
            BasicMesh::VHandle vh[3];
            vh[0] = BasicMesh::VHandle(fc->vertex(0)->info().idx);
            vh[1] = BasicMesh::VHandle(fc->vertex(1)->info().idx);
            vh[2] = BasicMesh::VHandle(fc->vertex(2)->info().idx);
            FaceHandle f = add_face(vh[0], vh[1], vh[2]);
            data(f).rid = fc->info().color_label;
            if(data(f).rid > maxregion)
            {
                maxregion = data(f).rid;
            }
        }
    }
    // 存下不同的 region
    m_Regions.resize(maxregion + 1);
    for(BasicMesh::FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        m_Regions[data(*fit).rid].push_back(*fit);
    }
    // 把兩邊的顏色填上，標上end point
    m_LinesVH.resize(m_Lines.Num());
    m_LinesHH.resize(m_Lines.Num());
    for(int32 i = 0; i < m_Lines.Num(); ++i)
    {
        VHandles& tvhs = m_LinesVH[i];
        HHandles& thhs = m_LinesHH[i];
        ULineV2& line = *m_Lines[i];
        BasicMesh::Point now(line[0].X, line[0].Y, 0);
        for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
        {
            if((point(*vit) - now).sqrnorm() < 0.01)
            {
                tvhs.push_back(*vit);
                break;
            }
        }
        int32 k = 0;
        ints idxs;
        TArray<FVector2D> pts;
        for(int32 j = 1; j < line.Num(); ++j)
        {
            idxs.Add(j);
            k++;
            if(k > line.Num() * 3)
            {
                break;
            }
            now = BasicMesh::Point(line[j].X, line[j].Y, 0);
            BasicMesh::Point lastp = BasicMesh::Point(line[j - 1].X, line[j - 1].Y, 0);
            BasicMesh::VHandle last = tvhs.back();
            float initdis = 10000;
            float mindis = initdis;
            int32 minidx = -1;
            int32 ii = 0;
            float pointdis = initdis;
            ii = 0;
            for(VVIter vvit = vv_iter(last); vvit.is_valid(); ++vvit, ii++)
            {
                Point tp = point(*vvit);
                FVector2D p(tp[0], tp[1]);
                float dis = (tp - now).norm() + (tp - lastp).norm();
                if(abs(dis - mindis) < 0.1)
                {
                    float npointdis = (tp - now).norm();
                    if(npointdis < pointdis)
                    {
                        pointdis = npointdis;
                        mindis = dis;
                        minidx = ii;
                    }
                }
                else if(dis < mindis)
                {
                    pointdis = (tp - now).norm();
                    mindis = dis;
                    minidx = ii;
                }
            }
            if(pointdis > 0.3)
            {
                --j;
            }
            if(minidx >= 0)
            {
                ii = 0;
                for(VVIter vvit = vv_iter(last); vvit.is_valid(); ++vvit)
                {
                    if(ii == minidx)
                    {
                        thhs.push_back(find_halfedge(last, *vvit));
                        data(edge_handle(thhs.back())).constraint = 1;
                        FHandle tfh = face_handle(thhs.back());
                        if(tfh.is_valid())
                        {
							data(tfh).cid = m_i2s[i]->right[0];
                        }
                        FHandle ofh = opposite_face_handle(thhs.back());
                        if(ofh.is_valid())
                        {
							data(ofh).cid = m_i2s[i]->left[0];
                        }
                        tvhs.push_back(*vvit);
                        Point pp = point(*vvit);
						pts.Add(FVector2D(pp[0], pp[1]));
                    }
                    ++ii;
                }
            }
        }
    }
    for(int32 i = 0; i < m_Regions.size(); ++i)
    {
        ints rnum;
        ints count;
		for (int32 j = 0; j < m_Regions[i].size(); ++j)
        {
            int32 cid = data(m_Regions[i][j]).cid;
            if(cid != -1)
            {
				if (rnum.Contains(cid))
                {
					count[rnum.Find(cid)]++;
                }
                else
                {
                    rnum.Add(cid);
                    count.Add(1);
                }
            }
        }
        if(count.Num() > 0)
        {
			int32 maxid = std::max_element(count.GetData(), count.GetData() + count.Num()) - count.GetData();
            for(int32 j = 0; j < m_Regions[i].size(); ++j)
            {
                data(m_Regions[i][j]).cid = rnum[maxid];
            }
        }
    }
}

void PicMesh::MakeColor1()
{
    m_Trangles.Reset();
	FColorTriData t;
    TArray<FVector> colors;
    for(int32 i = 0; i < m_Regions.size(); ++i)
    {
        colors.Add(FVector(rand() % 256, rand() % 256, rand() % 256));
    }
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        int32 tid = data(*fit).cid;
        int32 rid = data(*fit).rid;
        data(*fit).c2 = colors[rid];
        int32 c = 0;
        for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
        {
            Point p = point(*fvit);
            t.p[c][0] = p[0];
			t.p[c][1] = m_h - p[1];
//             if(tid > 0 && m_ColorConstraint.Num() >= tid)
//             {
//                 t.c[c] = m_ColorConstraint[tid - 1].GetColorVector3(p[0], p[1]);
//             }
//             else
            if(rid >= 0 && rid < colors.Num())
            {
                t.c[c] = colors[rid];
            }
            ++c;
        }
        m_Trangles.Add(t);
    }
}

void PicMesh::MakeColor2()
{
    m_Trangles.Reset();
    FColorTriData t;
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        int32 tid = data(*fit).cid;
        int32 c = 0;
        if(tid > 0)
        {
            for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
            {
                Point p = point(*fvit);
                t.p[c][0] = p[0];
                t.p[c][1] = p[1];
                t.c[c] = m_ColorConstraint[tid - 1]->GetColor(p[0], p[1]);
                ++c;
            }
            m_Trangles.Add(t);
        }
    }
}

void PicMesh::MappingMesh(PicMesh& pm, float x, float y)
{
    BasicMesh::VHandle lastV(pm.n_vertices() / 2);
	for (BasicMesh::VIter pvit = this->vertices_begin(); pvit != this->vertices_end(); ++pvit)
    {
		BasicMesh::Point now = this->point(*pvit);
        now[0] += x;
        now[1] += y;
        float tdis = (pm.point(lastV) - now).sqrnorm();
        float mindis = tdis;
        for(; tdis > 1;)
        {
            int32 minidx = -1;
            for(VVIter vvit = pm.vv_iter(lastV); vvit.is_valid(); ++vvit)
            {
                Point tp = pm.point(*vvit);;
                float dis = (tp - now).sqrnorm();
                if(dis < mindis)
                {
                    mindis = dis;
                    minidx = (*vvit).idx();
                }
            }
            if(minidx >= 0)
            {
                tdis = mindis;
                lastV = BasicMesh::VHandle(minidx);
            }
            else
            {
                break;
            }
        }
		this->data(*pvit).mapid = lastV.idx();
    }
    for(BasicMesh::FIter pfit = faces_begin(); pfit != faces_end(); ++pfit)
    {
        BasicMesh::Point now = MidPoint(*pfit);
        now[0] += x;
        now[1] += y;
        BasicMesh::VHandle lastV(pm.n_vertices() / 2);
        float tdis = (pm.point(lastV) - now).sqrnorm();
        float mindis = tdis;
        for(; tdis > 1;)
        {
            int32 minidx = -1;
            for(VVIter vvit = pm.vv_iter(lastV); vvit.is_valid(); ++vvit)
            {
                Point tp = pm.point(*vvit);;
                float dis = (tp - now).sqrnorm();
                if(dis < mindis)
                {
                    mindis = dis;
                    minidx = (*vvit).idx();
                }
            }
            if(minidx >= 0)
            {
                tdis = mindis;
                lastV = BasicMesh::VHandle(minidx);
            }
            else
            {
                break;
            }
        }
        int32 minidx = -1;
        int32 ridx = -1;
        mindis = 999999;
        for(VFIter ffit = pm.vf_iter(lastV); ffit.is_valid(); ++ffit)
        {
            Point tp = pm.MidPoint(*ffit);
            float dis = (tp - now).sqrnorm();
            if(dis < mindis)
            {
                mindis = dis;
                minidx = (*ffit).idx();
                ridx = pm.data(*ffit).rid;
            }
        }
        data(*pfit).mapid = ridx;
        BasicMesh::FHandle lastF(minidx);
        if(minidx >= 0)
        {
            //data(*pfit).c2 = pm.data(lastF).c2;
            data(*pfit).rid2 = pm.data(lastF).rid;
            data(*pfit).cid = minidx;
        }
    }
    
	m_MapingRegionIDs.SetNumZeroed(m_Regions.size());
    for(int32 i = 0; i < m_Regions.size(); ++i)
    {
        FHandles& region = m_Regions[i];
        if(region.size() == 0)
        {
            continue;
        }
        int32 step = region.size() / 20;
        if(step < 1)
        {
            step = 1;
        }
        TArray<FVector> colors;
        ints idxs;
        ints cuts;
        int32 count = 0;
        for(int32 j = 0; j < region.size(); j += step)
        {
            bool hasrepeat = false;
            int32 q = 0;
            for(; q < idxs.Num(); ++q)
            {
                if(data(region[j]).rid2 == idxs[q])
                {
                    hasrepeat = true;
                    break;;
                }
            }
            if(!hasrepeat)
            {
                idxs.Add(data(region[j]).rid2);
                colors.Add(data(region[j]).c2);
                cuts.Add(1);
            }
            else
            {
                cuts[q]++;
            }
            count++;
        }
        FVector dst;
        m_MapingRegionIDs[i] = -1;
		int32 maxe = std::max_element(cuts.GetData(), cuts.GetData() + cuts.Num()) - cuts.GetData();
        if(m_RegionAreas[i] > 20 && abs(m_RegionAreas[i] - idxs[maxe]) > m_RegionAreas[i])
        {
            float dis = 0;
            for(int32 j = 0; j < region.size(); ++j)
            {
                FHandle pmf(data(region[j]).cid);
                float ndis = FVector::Dist(data(region[j]).c2, pm.data(pmf).c2);
                dis += ndis;
            }
            dis /= region.size();
            //printf("color distance %f\n", dis);
            if(dis > 30)
            {
                m_MapingRegionIDs[i] = idxs[maxe];
            }
        }
//         for(int32 j = 0; j < region.size(); ++j)
//         {
//             data(region[j]).c2 = colors[maxe];
//         }
    }
	m_MapingCount.SetNumZeroed(pm.m_Regions.size());
    for(int32 i = 0; i < m_MapingRegionIDs.Num(); ++i)
    {
        if(m_MapingRegionIDs[i] != -1)
        {
            m_MapingCount[m_MapingRegionIDs[i]]++;
        }
    }
}

BasicMesh::Point PicMesh::MidPoint(FHandle fh)
{
    Point v[3];
    int32 k = 0;
    for(FVIter fv_it = fv_iter(fh); fv_it.is_valid() ; ++fv_it)
    {
        const Point& p = point(*fv_it);
        v[k++] = p;
    }
    Point res = (v[0] + v[1] + v[2]) * 0.33333;
    return res;
}

void PicMesh::MakeRegionLine(UcvMat* img, float lmax)
{
    // mark boundary constraint
    for(VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        Point p = point(*vit);
        if(p[0] == 0 || p[1] == 0 || p[0] == m_w || p[1] == m_h)
        {
            data(*vit).constraint = 1;
        }
    }
    // mark constraint point
    for(int32 i = 0; i < m_LinesVH.size(); ++i)
    {
        for(int32 j = 0; j < m_LinesVH[i].size(); ++j)
        {
            data(m_LinesVH[i][j]).constraint = 1;
            data(m_LinesVH[i][j]).lineid = i;
            data(m_LinesVH[i][j]).lineidx = j;
        }
    }
    // mark end point
    for(int32 i = 0; i < m_LinesVH.size(); ++i)
    {
        data(m_LinesVH[i].front()).end = true;
        data(m_LinesVH[i].back()).end = true;
    }
    for(int32 i = 0; i < m_LinesVH.size(); ++i)
    {
		ULineV2& nowline = *m_Lines[i];
        int32 last = m_LinesVH[i].size() - 1;
        FVector2D out;
        bool res;
        if(data(m_LinesVH[i][0]).constraint > 0)
        {
            Point dir;
            if(m_LinesVH[i].size() > 3)
            {
                FVector2D v = nowline[1] - nowline[2];
                dir = Point(v[0], v[1], 0);
            }
            else
            {
                FVector2D v = nowline[0] - nowline[1];
                dir = Point(v[0], v[1], 0);
            }
            dir.normalize();
            res = ConnectOneRingConstraint1(img, m_LinesVH[i][0], m_LinesVH[i][1], out, dir, lmax);
            if(res)
            {
                nowline.pts.Insert(out, 0);
            }
        }
        if(data(m_LinesVH[i][last]).constraint > 0)
        {
            int32 last2 = nowline.Num() - 1;
            Point dir, orig = point(m_LinesVH[i][last]);
            FVector2D vOrig(orig[0], orig[1]);
            FVector2D checkOrig = nowline[last2];
			if (FVector2D::DistSquared(vOrig, checkOrig) < 0.01)
            {
				if (m_LinesVH[i].size() > 3)
                {
                    FVector2D v = nowline[last2 - 1] - nowline[last2 - 2];
                    dir = Point(v[0], v[1], 0);
                }
                else
                {
                    FVector2D v = nowline[last2] - nowline[last2 - 1];
                    dir = Point(v[0], v[1], 0);
                }
                dir.normalize();
                res = ConnectOneRingConstraint1(img, m_LinesVH[i][last], m_LinesVH[i][last - 1], out, dir, lmax);
                if(res)
                {
					nowline.pts.Add(out);
                }
            }
        }
    }
}

bool PicMesh::ConnectOneRingConstraint1(UcvMat* img, VHandle vh, VHandle lastvh, FVector2D& out, Point dir, float lmax)
{
    VHandles ends;
    VHandles constraints;
    VHandles alls;
    const float GradientThreshold = 1000;
    const float LINE_THRESHOLD = 5;
    bool isok = false;
    for(VVIter vvit = vv_iter(vh); vvit.is_valid(); ++vvit)
    {
        if(*vvit != lastvh)
        {
            if(data(*vvit).end)
            {
                Point px = point(*vvit) - point(vh);
                if(px.length() < lmax)
                {
                    ends.push_back(*vvit);
                    alls.push_back(*vvit);
                }
            }
            else if(data(*vvit).constraint > 0)
            {
                Point px = point(*vvit) - point(vh);
                if(px.length() < lmax)
                {
                    if(data(*vvit).lineid == data(vh).lineid)
                    {
                        if(abs(data(*vvit).lineidx - data(vh).lineidx) > LINE_THRESHOLD)
                        {
                            constraints.push_back(*vvit);
                            alls.push_back(*vvit);
                        }
                    }
                    else
                    {
                        constraints.push_back(*vvit);
                        alls.push_back(*vvit);
                    }
                }
            }
        }
    }
    // 移除同一條線的constraint點
    if(ends.size() > 0 && constraints.size() > 0)
    {
        for(int32 i = 0; i < ends.size(); ++i)
        {
            for(int32 j = 0; j < constraints.size(); ++j)
            {
                if(data(ends[i]).lineid == data(constraints[j]).lineid)
                {
                    constraints.erase(constraints.begin() + j);
                    j--;
                }
            }
        }
    }
    // 直走 距離小於3
    if(alls.size() > 0)
    {
        Point p = dir;
        p.normalize();
        FVector2D v(p[0], p[1]);
        floats lens;
        for(int32 i = 0; i < alls.size(); ++i)
        {
            Point px = point(alls[i]) - point(vh);
            px.normalize();
            FVector2D v2(px[0], px[1]);
			lens.Add(FVector2D::Distance(v, v2));
        }
        float minlen = 999;
        int32 idx = 0;
        for(int32 i = 0; i < lens.Num(); ++i)
        {
            if(minlen > lens[i])
            {
                minlen = lens[i];
                idx = i;
            }
        }
        Point op = point(alls[idx]);
        float dis = (op - point(vh)).length();
        if(dis < 3 && minlen < 0.3)
        {
            isok = true;
        }
        if(isok)
        {
            out = FVector2D(op[0], op[1]);
            //data(vh).end = false;
            data(alls[idx]).end = true;
            data(alls[idx]).constraint += 1;
            return true;
        }
    }
    // 有邊有點 共邊點先連
    if(ends.size() > 0 && constraints.size() > 0)
    {
        for(int32 i = 0; i < ends.size(); ++i)
        {
            if(data(ends[i]).constraint > 1)
            {
                Point p = point(ends[i]);
                float dis = (p - point(vh)).length();
                bool isok2 = true;
                for(int32 j = 0; j < constraints.size(); ++j)
                {
                    if((point(constraints[j]) - point(vh)).length() * 1.5 < dis)
                    {
                        isok2 = false;
                        break;
                    }
                }
                if(isok2 && dis < LINE_THRESHOLD)
                {
                    isok = true;
                }
                if(isok)
                {
                    out = FVector2D(p[0], p[1]);
                    data(vh).end = false;
                    data(ends[i]).constraint += 1;
                    return true;
                }
            }
        }
        for(int32 i = 0; i < ends.size(); ++i)
        {
            for(int32 j = 0; j < constraints.size(); ++j)
            {
                if(isConnection(ends[i], constraints[j]))
                {
                    Point p1 = dir;
                    p1.normalize();
                    Point p2 = point(ends[i]) - point(vh);
                    p2.normalize();
                    Point p3 = point(constraints[j]) - point(vh);
                    p3.normalize();
                    if((p1 - p3).length() < 0.7)
                    {
                        Point p = point(constraints[j]);
                        float dis = (p - point(vh)).length();
                        if(dis < LINE_THRESHOLD)
                        {
                            isok = true;
                        }
                        if(isok)
                        {
                            out = FVector2D(p[0], p[1]);
                            data(vh).end = false;
                            data(constraints[j]).end = true;
                            data(constraints[j]).constraint += 1;
                            return true;
                        }
                    }
                }
            }
        }
    }
    // 點只有一個 就近就連
    if(ends.size() == 1)
    {
        Point p = point(ends[0]);
        if((p - point(vh)).length() < 6)
        {
            out = FVector2D(p[0], p[1]);
            data(vh).end = false;
            data(ends[0]).end = false;
            return true;
        }
    }
    if(alls.size() > 0)
    {
        Point p = dir;
        p.normalize();
        FVector2D v(p[0], p[1]);
        floats lens;
        for(int32 i = 0; i < alls.size(); ++i)
        {
            Point px = point(alls[i]) - point(vh);
            px.normalize();
            FVector2D v2(px[0], px[1]);
			lens.Add(FVector2D::Distance(v, v2));
        }
        float minlen = 999;
        int32 idx = 0;
        for(int32 i = 0; i < lens.Num(); ++i)
        {
            if(minlen > lens[i])
            {
                minlen = lens[i];
                idx = i;
            }
        }
        Point op = point(alls[idx]);
        float dis = (op - point(vh)).length();
        if(dis > LINE_THRESHOLD)
        {
            if(minlen < 0.8)
            {
                isok = true;
            }
        }
        else
        {
            isok = true;
        }
        if(isok)
        {
            out = FVector2D(op[0], op[1]);
            //data(vh).end = false;
            data(alls[idx]).end = true;
            data(alls[idx]).constraint += 1;
            return true;
        }
    }
    if(ends.size() > 1)
    {
        Point p = dir;
        p.normalize();
        FVector2D v(p[0], p[1]);
        floats lens;
        for(int32 i = 0; i < ends.size(); ++i)
        {
            Point px = point(ends[i]) - point(vh);
            px.normalize();
            FVector2D v2(px[0], px[1]);
			lens.Add(FVector2D::Distance(v, v2));
        }
        float minlen = 99;
        int32 idx = 0;
        for(int32 i = 0; i < lens.Num(); ++i)
        {
            if(minlen > lens[i])
            {
                minlen = lens[i];
                idx = i;
            }
        }
        Point op = point(ends[idx]);
        out = FVector2D(op[0], op[1]);
        data(vh).end = false;
        data(ends[idx]).end = false;
        return true;
    }
    return false;
}



void PicMesh::SetSize(int32 w, int32 h)
{
    m_w = w;
    m_h = h;
}

bool PicMesh::isConnection(VHandle vh1, VHandle vh2)
{
    for(VVIter vvit = vv_iter(vh1); vvit.is_valid(); ++vvit)
    {
        if(*vvit == vh2)
        {
            return true;
        }
    }
    return false;
}

void PicMesh::BuildColorModels(UcvMat* img)
{
    // mark boundary constraint
    for(int32 i = 0; i < m_LinesHH.size(); ++i)
    {
        HHandles& nowhh = m_LinesHH[i];
        for(int32 j = 0; j < nowhh.size(); ++j)
        {
            data(edge_handle(nowhh[j])).constraint = true;
        }
    }
    for(VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        Point p = point(*vit);
        if(p[0] == 0 || p[1] == 0 || p[0] == m_w || p[1] == m_h)
        {
            data(*vit).constraint = 1;
        }
    }
    // mark constraint point
    for(int32 i = 0; i < m_LinesVH.size(); ++i)
    {
		for (int32 j = 0; j < m_LinesVH[i].size(); ++j)
        {
            data(m_LinesVH[i][j]).constraint = 1;
            data(m_LinesVH[i][j]).lineid = i;
            data(m_LinesVH[i][j]).lineidx = j;
        }
    }
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        data(*vit).use = false;
    }
    m_ColorConstraint.Reset();
    for(int32 i = 0; i < m_Regions.size(); ++i)
    {
		m_ColorConstraint.Add(NewObject<UFlatVec3MeshLinear>());
		UFlatVec3MeshLinear& cc = *m_ColorConstraint.Last();
        FHandles& region = m_Regions[i];
        for(int32 j = 0; j < region.size(); ++j)
        {
            for(FEIter fe_itr = fe_iter(region[j]) ; fe_itr.is_valid(); ++fe_itr)
            {
                BasicMesh::HalfedgeHandle _hedge = halfedge_handle(*fe_itr, 1);
                FVector cx;
                Point p1 = point(from_vertex_handle(_hedge));
                Point p2 = point(to_vertex_handle(_hedge));
                p1 = (p1 + p2) * 0.5;
                FVector2D pp(p1[0], p1[1]);
                bool res = GetLighterColor(img, from_vertex_handle(_hedge),
                                           to_vertex_handle(_hedge), cx);
                if(res)
                {
                    cc.AddPoint(pp[0], pp[1], cx);
                }
            }
            Point p = MidPoint(region[j]);
            FVector c = img->GetBilinearColor(p[0], p[1]);
            float t = c[0];
            c[0] = c[2];
            c[2] = t;
            cc.AddPoint(p[0], p[1], c);
            for(FVIter fvit = fv_iter(region[j]); fvit.is_valid(); ++fvit)
            {
                if(data(*fvit).constraint == 0 && !data(*fvit).use)
                {
                    data(*fvit).use = true;
                    Point p2 = point(*fvit);
                    FVector c = img->GetBilinearColor(p2[0] - 0.5, p2[1] - 0.5);
                    float t = c[0];
                    c[0] = c[2];
                    c[2] = t;
                    cc.AddPoint(p2[0], p2[1], c);
                }
            }
        }
        cc.BuildModel();
    }
}

void PicMesh::BuildColorModels()
{
	// mark boundary constraint
	for (int32 i = 0; i < m_LinesHH.size(); ++i)
	{
		HHandles& nowhh = m_LinesHH[i];
		for (int32 j = 0; j < nowhh.size(); ++j)
		{
			data(edge_handle(nowhh[j])).constraint = true;
		}
	}
	for (VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
	{
		Point p = point(*vit);
		if (p[0] == 0 || p[1] == 0 || p[0] == m_w || p[1] == m_h)
		{
			data(*vit).constraint = 1;
		}
	}
	// mark constraint point
	for (int32 i = 0; i < m_LinesVH.size(); ++i)
	{
		for (int32 j = 0; j < m_LinesVH[i].size(); ++j)
		{
			data(m_LinesVH[i][j]).constraint = 1;
			data(m_LinesVH[i][j]).lineid = i;
			data(m_LinesVH[i][j]).lineidx = j;
		}
	}
	for (BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
	{
		data(*vit).use = false;
	}
	m_ColorConstraint.Reset();
	for (int32 i = 0; i < m_Regions.size(); ++i)
	{
		m_ColorConstraint.Add(NewObject<UFlatVec3MeshLinear>());
		UFlatVec3MeshLinear& cc = *m_ColorConstraint.Last();
		FHandles& region = m_Regions[i];
		for (int32 j = 0; j < region.size(); ++j)
		{
			if (data(region[j]).c2 != FVector::ZeroVector)
			{
				Point p = MidPoint(region[j]);
				cc.AddPoint(p[0], p[1], data(region[j]).c2);
				FVector c2 = data(region[j]).c2;
			}
			for (FVIter fvit = fv_iter(region[j]); fvit.is_valid(); ++fvit)
			{
				if (data(*fvit).c2 != FVector::ZeroVector)
				{
					Point p2 = point(*fvit);
					cc.AddPoint(p2[0], p2[1], data(*fvit).c2);
					FVector c2 = data(*fvit).c2;
				}
			}
		}
		cc.BuildModel();
	}
}

bool PicMesh::GetLighterColor(UcvMat* img, VHandle vh1, VHandle vh2, FVector& c)
{
    if(data(vh1).constraint > 0 && data(vh2).constraint > 0 && !data(edge_handle(find_halfedge(vh1, vh2))).constraint)
    {
		BasicMesh::Point p1 = point(vh1);
		BasicMesh::Point p2 = point(vh2);
        FVector2D v1(p1[0] + 1, p1[1]);
        FVector2D v2(p2[0], p2[1]);
        FVector2D step = (v2 - v1) / 10;
        float maxlight = 0;
        for(int32 i = 1; i < 9; ++i)
        {
            FVector2D dst = v1 + step * i;
            FVector tmp = img->GetBilinearColor(dst[0] - 0.5, dst[1] - 0.5);
            std::swap(tmp[0], tmp[2]);
            float light = GetLight(tmp);
            if(maxlight < light)
            {
                maxlight = light;
                c = tmp;
            }
        }
//         std::sort(colors.begin(), colors.end());
//         c = colors[colors.Num() * 0.8];
        return true;
    }
    return false;
}

void PicMesh::MakeColor6(UcvMat* img)
{
	img->ConvertMat(EcvMatEnum::UC_BGR);
	m_DifferentRegionIDs.SetNumZeroed(m_Regions.size());
    // mark boundary constraint
    for(VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        Point p = point(*vit);
        if(p[0] == 0 || p[1] == 0 || p[0] == m_w || p[1] == m_h)
        {
            data(*vit).constraint = 1;
        }
    }
    // mark constraint point
    for(int32 i = 0; i < m_LinesVH.size(); ++i)
    {
        for(int32 j = 0; j < m_LinesVH[i].size(); ++j)
        {
            data(m_LinesVH[i][j]).constraint = 1;
        }
    }
    m_Trangles.Reset();
    FColorTriData t;
    // get color far constraint
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            Point p = point(*vit);
            FVector c2 = img->GetBilinearColor(p[0] - 0.5, p[1] - 0.5);
			UE_LOG(Vectorizing, Display, TEXT("Pos (%.1f, %.1f) Color R: %.1f G: %.1f B: %.1f"), p[0], p[1], c2.X, c2.Y, c2.Z);
            std::swap(c2[0], c2[2]);
            if(c2 == FVector::ZeroVector)
            {
                c2.X = 1;
            }
            data(*vit).c2 = c2;
			m_DifferentRegionIDs[data(*vf_iter(*vit)).rid]++;
        }
    }
    // blur c2
    lightblurC2();
    lightblurC2();
    lightblurC2();

    //  removeblackC2();
    //  removeblackC2();
    //  removeblackC2();
    BuildColorModels();
    MarkColoredFace();
    FillConstraintRegion();
 	GetConstraintFaceColor(img);
	blurFaceC2();
	
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        int32 rid = data(*fit).rid;
        if(rid >= 0 && rid < m_MapingRegionIDs.Num() && m_MapingRegionIDs[rid] <= 0)
        {
            continue;
        }
        int32 c = 0;
        BasicMesh::Point mid = MidPoint(*fit);
        for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
        {
            Point p = point(*fvit);
            t.p[c][0] = p[0];
            t.p[c][1] = m_h - p[1];
            if(data(*fit).mark > 0 && rid >= 0 && m_ColorConstraint.Num() > rid)
            {
                t.c[c] = m_ColorConstraint[rid]->GetColor2Point(p[0], p[1], mid[0], mid[1]);
            }
            else if(data(*fit).mark < 0)
            {
                t.c[c] = data(*fit).c2;
            }
            ++c;
        }
        if(data(*fit).mark != 0)
        {
            m_Trangles.Add(t);
        }
    }
}

void PicMesh::blurC2()
{
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            int32 count = 1;
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

void PicMesh::lightblurC2()
{
    for(BasicMesh::VIter vit = vertices_begin(); vit != vertices_end(); ++vit)
    {
        if(data(*vit).constraint == 0)
        {
            int32 count = 1;
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
            float light = GetLight(blur);
            count = 1;
            blur = data(*vit).c2;
            for(VVIter vv_itr = vv_iter(*vit) ; vv_itr.is_valid(); ++vv_itr)
            {
                if(data(*vv_itr).constraint == 0)
                {
                    if(GetLight(data(*vv_itr).c2) > light)
                    {
                        count += 5;
                        blur += data(*vv_itr).c2 * 5;
                    }
                    else
                    {
                        count++;
                        blur += data(*vv_itr).c2;
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
            data(*vit).c2 = data(*vit).c3;
        }
    }
}


void PicMesh::GetConstraintFaceColor(UcvMat* img)
{
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        if(data(*fit).mark == 0 && m_DifferentRegionIDs[data(*fit).rid] == 0)
        {
            for(FEIter fe_itr = fe_iter(*fit) ; fe_itr.is_valid(); ++fe_itr)
            {
                BasicMesh::HalfedgeHandle _hedge = halfedge_handle(*fe_itr, 1);
                FVector cx = FVector::ZeroVector;
                Point p1 = point(from_vertex_handle(_hedge));
                Point p2 = point(to_vertex_handle(_hedge));
                p1 = (p1 + p2) * 0.5;
                FVector2D pp(p1[0], p1[1]);
                bool res = GetLighterColor(img, from_vertex_handle(_hedge),
                                           to_vertex_handle(_hedge), cx);
                if(res)
                {
                    if(cx == FVector::ZeroVector)
                    {
                        cx.X = 1;
                    }
                    data(*fit).c2 = cx;
                    data(*fit).mark = -2;
                    break;
                }
            }
        }
    }
}

void PicMesh::blurFaceC2()
{
	FVector allavg = FVector::ZeroVector;
	int32 allcount = 0;
    for(BasicMesh::FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        if(data(*fit).c2 != FVector::ZeroVector)
        {
            int32 count = 1;
            FVector blur = data(*fit).c2;
            for(FFIter ff_itr = ff_iter(*fit) ; ff_itr.is_valid(); ++ff_itr)
            {
                if(data(*ff_itr).c2 != FVector::ZeroVector)
                {
                    count++;
                    blur += data(*ff_itr).c2;
                }
            }
            blur /= count;
            data(*fit).c3 = blur;
			allavg += blur;
			allcount++;
        }
    }
	allavg /= allcount;
    for(BasicMesh::FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        if(data(*fit).c2 != FVector::ZeroVector && m_DifferentRegionIDs[data(*fit).rid] == 0)
        {
			data(*fit).c2 = allavg;
        }
    }
}

void PicMesh::MarkColoredFace()
{
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        int32 rid = data(*fit).rid;
        int32 c = 0;
        int32 c0 = 0;
        BasicMesh::Point mid = MidPoint(*fit);
        for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
        {
            if(data(*fvit).constraint == 0)
            {
                c0++;
            }
        }
        if(c0 > 0)
        {
            data(*fit).mark = c0;
            if(c0 < 3)
            {
				FVector sumc = FVector::ZeroVector;
                for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
                {
                    if(data(*fvit).constraint == 0)
                    {
                        sumc += data(*fvit).c2;
                    }
                }
                sumc /= c0;
                data(*fit).c2 = sumc;
            }
        }
    }
}

void PicMesh::FillConstraintRegion()
{
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        if(data(*fit).mark == 0)
        {
            for(FaceHalfedgeIter feit = fh_iter(*fit); feit.is_valid(); ++feit)
            {
                if(data(edge_handle(*feit)).constraint == 0)
                {
                    BasicMesh::HalfedgeHandle _hedge = *feit;
                    FaceHandle fh = opposite_face_handle(_hedge);
                    if(fh.is_valid() && data(fh).mark < 3 && data(fh).mark != 0)
                    {
                        data(*fit).c2 = data(fh).c2;
                        data(*fit).mark = -1;
                        break;
                    }
                }
            }
            if(data(*fit).mark == -1)
            {
                FHandles que;
                for(FaceHalfedgeIter feit = fh_iter(*fit); feit.is_valid(); ++feit)
                {
                    if(data(edge_handle(*feit)).constraint == 0)
                    {
                        BasicMesh::HalfedgeHandle _hedge = *feit;
                        FaceHandle fh = opposite_face_handle(_hedge);
                        if(fh.is_valid() && data(fh).mark == 0)
                        {
                            que.push_back(fh);
                            data(fh).c2 = data(*fit).c2;
                        }
                    }
                }
                while(!que.empty())
                {
                    FHandle now = que.back();
                    que.pop_back();
                    for(FaceHalfedgeIter feit = fh_iter(now); feit.is_valid(); ++feit)
                    {
                        if(edge_handle(*feit).is_valid() &&
                                data(edge_handle(*feit)).constraint == 0)
                        {
                            BasicMesh::HalfedgeHandle _hedge = *feit;
                            FaceHandle fh = opposite_face_handle(_hedge);
                            if(fh.is_valid() && data(fh).mark == 0)
                            {
                                que.push_back(fh);
                                data(fh).c2 = data(*fit).c2;
                                data(fh).mark = -1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void PicMesh::SetRegionColor(UcvMat* img)
{
    m_RegionColor.SetNumZeroed(m_Regions.size());
    for(int32 i = 0; i < m_Regions.size(); ++i)
    {
        if(m_Regions[i].size() > 1)
        {
            FVector nc;
            float scalar = 1.f / 30.f;
            std::set<int32> intsetv;
			float step = (m_Regions[i].size() - 1) / 30.f;
			for (float j = 0; j < m_Regions[i].size(); j += step)
            {
                Point p = MidPoint(m_Regions[i][(int32)j]);
                p[0] += 0.5;
                p[1] += 0.5;
                nc += scalar * img->GetBilinearColor(p[0], p[1]);
            }
            m_RegionColor[i] = nc;
        }
        else
        {
			if (m_Regions[i].size() != 0)
            {
                Point p = MidPoint(m_Regions[i][0]);
                p[0] += 0.5;
                p[1] += 0.5;
                m_RegionColor[i] = img->GetBilinearColor(p[0], p[1]);
            }
        }
    }
}

void PicMesh::MakeColorX9(int32 id)
{
    FColorTriData t;
	m_Trangles.Reset();
    for(FIter fit = faces_begin(); fit != faces_end(); ++fit)
    {
        if(data(*fit).rid == id)
        {
            int32 c = 0;
            for(FVIter fvit = fv_iter(*fit); fvit.is_valid(); ++fvit)
            {
                Point p = point(*fvit);
                t.p[c][0] = p[0];
                t.p[c][1] = p[1];
                t.c[c] = data(*fit).c2;
                ++c;
            }
            m_Trangles.Add(t);
        }
    }
}

int32 PicMesh::GetRegionId(float x, float y)
{
    BasicMesh::VHandle lastV(n_vertices() / 2);
    BasicMesh::Point now(x , y);
    BasicMesh::FHandle dst(-1);
    float tdis = (point(lastV) - now).sqrnorm();
    float mindis = tdis;
    for(; tdis > 1;)
    {
        int32 minidx = -1;
        for(VVIter vvit = vv_iter(lastV); vvit.is_valid(); ++vvit)
        {
            Point tp = point(*vvit);;
            float dis = (tp - now).sqrnorm();
            if(dis < mindis)
            {
                mindis = dis;
                minidx = (*vvit).idx();
            }
        }
        if(minidx >= 0)
        {
            tdis = mindis;
            lastV = BasicMesh::VHandle(minidx);
        }
        else
        {
            break;
        }
    }
    for(VFIter vfit = vf_iter(lastV); vfit.is_valid(); ++vfit)
    {
        float dis = (MidPoint(*vfit) - now).sqrnorm();
        if(dis < mindis)
        {
            mindis = dis;
            dst = *vfit;
        }
    }

    if(dst.idx() >= 0)
    {
        printf("mapping to key frame index %d \n", data(dst).rid2);
        return data(dst).rid2;
    }
    return -1;
}

void PicMesh::ComputeRegion()
{
	m_RegionAreas.SetNumZeroed(m_Regions.size());
	for (int32 i = 0; i < m_Regions.size(); ++i)
	{
		m_RegionAreas[i] = m_Regions[i].size();
	}
}




