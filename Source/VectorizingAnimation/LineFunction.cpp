// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineFunction.h"
#include "LineV2.h"
#include "LineEnd.h"
#include <cmath>
#include <vector>
#include "GeneralDef.h"

TArray<ULineEnd*> ULineFunction::LinkLineEnds180(TArray<ULineEnd*> les, float distance, float angle)
{
    float sdistance = distance * distance;
    for(int32 i = 0; i < les.Num(); ++i)
    {
        ULineEnd& le1 = *les[i];
        for(int32 j = 0; j < les.Num(); ++j)
        {
            if(i != j)
            {
                bool haslink = false;
                ULineEnd& le2 = *les[j];
                // find begin link
                if(FVector2D::DistSquared(le1.beg, le2.beg) < sdistance)
                {
                    if(CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::BEGIN_TO_BEGIN, angle))
                    {
                        le1.beglinks.Add(j);
                        le2.beglinks.Add(i);
                        le1.beglink = &le2;
                        le2.beglink = &le1;
                        haslink = true;
                    }
                }
                else if(FVector2D::DistSquared(le1.beg, le2.end) < sdistance)
                {
                    if(CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::BEGIN_TO_END, angle))
                    {
                        le1.beglinks.Add(j);
                        le2.endlinks.Add(i);
                        le1.beglink = &le2;
                        le2.endlink = &le1;
                        haslink = true;
                    }
                }
                // find end link
                if(FVector2D::DistSquared(le1.end, le2.beg) < sdistance)
                {
                    if(CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::END_TO_BEGIN, angle))
                    {
                        le1.endlinks.Add(j);
                        le2.beglinks.Add(i);
                        le1.endlink = &le2;
                        le2.beglink = &le1;
                        haslink = true;
                    }
                }
                else if(FVector2D::DistSquared(le1.end, le2.end) < sdistance)
                {
                    if(CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::END_TO_END, angle))
                    {
                        le1.endlinks.Add(j);
                        le2.endlinks.Add(i);
                        le1.endlink = &le2;
                        le2.endlink = &le1;
                        haslink = true;
                    }
                }
                if(haslink)
                {
                    if(le1.linkIdx1)
                    {
                        le1.linkIdx2 = &le2;
                    }
                    else
                    {
                        le1.linkIdx1 = &le2;
                    }
                    if(le2.linkIdx1)
                    {
                        le2.linkIdx2 = &le1;
                    }
                    else
                    {
                        le2.linkIdx1 = &le1;
                    }
                }
            }
        }
    }
    return les;
}


bool ULineFunction::CheckLinkEnd_Similarity180(const ULineEnd* plhs, const ULineEnd* prhs, ELinkMethod c, float angle)
{
    const ULineEnd& lhs = *plhs;
    const ULineEnd& rhs = *prhs;
    float rangleBeg = fmod(rhs.angleBeg + 180, 360);
    float rangleEnd = fmod(rhs.angleEnd + 180, 360);
    switch(c)
    {
    case ELinkMethod::BEGIN_TO_BEGIN:
        if(CheckAngle(lhs.angleBeg, rangleBeg, angle))
        {
            FVector2D p1 = rhs.beg - lhs.beg;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(CheckAngle(lhs.angleBeg, angle1, angle) &&
                    CheckAngle(rangleBeg, angle1, angle))
            {
                return true;
            }
        }
        break;
    case ELinkMethod::BEGIN_TO_END:
        if(CheckAngle(lhs.angleBeg, rangleEnd, angle))
        {
            FVector2D p1 = rhs.end - lhs.beg;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(CheckAngle(lhs.angleBeg, angle1, angle) &&
                    CheckAngle(rangleEnd, angle1, angle))
            {
                return true;
            }
        }
        break;
    case ELinkMethod::END_TO_BEGIN:
        if(CheckAngle(lhs.angleEnd, rangleBeg, angle))
        {
            FVector2D p1 = rhs.beg - lhs.end;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(CheckAngle(lhs.angleEnd, angle1, angle) &&
                    CheckAngle(rangleBeg, angle1, angle))
            {
                return true;
            }
        }
        break;
    case ELinkMethod::END_TO_END:
        if(CheckAngle(lhs.angleEnd, rangleEnd, angle))
        {
            FVector2D p1 = rhs.end - lhs.end;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(CheckAngle(lhs.angleEnd, angle1, angle) &&
                    CheckAngle(rangleEnd, angle1, angle))
            {
                return true;
            }
        }
        break;
    }
    return false;
}

bool ULineFunction::CheckLinkEnd_Similarity(const ULineEnd* plhs, const ULineEnd* prhs, ELinkMethod c, float angle)
{
    const ULineEnd& lhs = *plhs;
    const ULineEnd& rhs = *prhs;
    float rangleBeg = rhs.angleBeg + 360;
    float rangleEnd = rhs.angleEnd + 360;
    float rangleBeg2 = fmod(rangleBeg, 360);
    float rangleEnd2 = fmod(rangleEnd, 360);
    switch(c)
    {
    case ELinkMethod::BEGIN_TO_BEGIN:
        if(abs(lhs.angleBeg - rangleBeg) < angle || abs(lhs.angleBeg - rangleBeg2) < angle)
        {
            FVector2D p1 = rhs.beg - lhs.beg;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(abs(lhs.angleBeg - angle1) < angle)
            {
                return true;
            }
        }
        break;
    case ELinkMethod::BEGIN_TO_END:
        if(abs(lhs.angleBeg - rangleEnd) < angle || abs(lhs.angleBeg - rangleEnd2) < angle)
        {
            FVector2D p1 = rhs.end - lhs.beg;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(abs(lhs.angleBeg - angle1) < angle)
            {
                return true;
            }
        }
        break;
    case ELinkMethod::END_TO_BEGIN:
        if(abs(lhs.angleEnd - rangleBeg) < angle || abs(lhs.angleEnd - rangleBeg2) < angle)
        {
            FVector2D p1 = rhs.beg - lhs.end;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(abs(lhs.angleEnd - angle1) < angle)
            {
                return true;
            }
        }
        break;
    case ELinkMethod::END_TO_END:
        if(abs(lhs.angleEnd - rangleEnd) < angle || abs(lhs.angleEnd - rangleEnd2) < angle)
        {
            FVector2D p1 = rhs.end - lhs.end;
            float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
            if(abs(lhs.angleEnd - angle1) < angle)
            {
                return true;
            }
        }
        break;
    }
    return false;
}

TArray<ULineV2*> ULineFunction::ConnectLineEnds3(const TArray<ULineEnd*>& les, TArray<ULineV2*> pos)
{
    std::vector<bool> marked(pos.Num(), false);
    TArray<ULineV2*> newpos;
    for(auto it1 = les.CreateConstIterator(); it1; ++it1)
    {
        const ULineEnd& le1 = *(*it1);
        if(le1.beglinks.Num() > 0)
        {
            int32 nowid = le1.idx;
            if(marked[nowid])
            {
                continue;
            }
            ULineV2* uaddline = NewObject<ULineV2>();
            TArray<FVector2D>& addline = uaddline->pts;
            addline.Append(pos[nowid]->pts);
            marked[nowid] = true;
            nowid = le1.beglinks[0];
            while(nowid != -1 && !marked[nowid])
            {
                marked[nowid] = true;
                int32 next = nowid;
                const ULineV2* loopline = pos[nowid];
                float dis1 = FVector2D::Distance(loopline->pts[0], addline.Last());
                float dis2 = FVector2D::Distance(loopline->pts.Last(), addline.Last());
                if(dis1 < dis2)
                {
                    addline.Append(loopline->pts);
                    if(les[nowid]->endlinks.Num() > 0)
                    {
                        next = les[nowid]->endlinks.Last();
                    }
                }
                else
                {
//                  TArray<FVector2D> reverseline;
//                  reverseline.Reserve(loopline->pts.Num());
//                  for (int32 i = loopline->pts.Num() - 1; i >= 0; --i)
//                  {
//                      reverseline.Add(loopline->pts[i]);
//                  }
                    std::reverse((FVector2D*)(loopline->pts.GetData()),
                                 (FVector2D*)(loopline->pts.GetData() + loopline->pts.Num()));
                    addline.Append(loopline->pts);
                    if(les[nowid]->beglinks.Num() > 0)
                    {
                        next = les[nowid]->beglinks[0];
                    }
                }
                if(next == nowid)
                {
                    nowid = -1;
                }
            }
            newpos.Add(uaddline);
        }
        else if(le1.endlinks.Num() > 0)
        {
            int32 nowid = le1.idx;
            if(marked[nowid])
            {
                continue;
            }
            ULineV2* uaddline = NewObject<ULineV2>();
            TArray<FVector2D>& addline = uaddline->pts;
            addline.Append(pos[nowid]->pts);
            marked[nowid] = true;
            nowid = le1.endlinks[0];
            while(nowid != -1 && !marked[nowid])
            {
                marked[nowid] = true;
                int32 next = nowid;
                const ULineV2* loopline = pos[nowid];
                float dis1 = FVector2D::Distance(loopline->pts[0], addline.Last());
                float dis2 = FVector2D::Distance(loopline->pts.Last(), addline.Last());
                if(dis1 < dis2)
                {
                    addline.Append(loopline->pts);
                    if(les[nowid]->endlinks.Num() > 0)
                    {
                        next = les[nowid]->endlinks.Last();
                    }
                }
                else
                {
//                  TArray<FVector2D> reverseline;
//                  reverseline.Reserve(loopline->pts.Num());
//                  for (int32 i = loopline->pts.Num() - 1; i >= 0; --i)
//                  {
//                      reverseline.Add(loopline->pts[i]);
//                  }
                    std::reverse((FVector2D*)(loopline->pts.GetData()),
                                 (FVector2D*)(loopline->pts.GetData() + loopline->pts.Num()));
                    addline.Append(loopline->pts);
                    if(les[nowid]->beglinks.Num() > 0)
                    {
                        next = les[nowid]->beglinks[0];
                    }
                }
                if(next == nowid)
                {
                    nowid = -1;
                }
            }
            newpos.Add(uaddline);
        }
    }
    for(int32 i = 0; i < les.Num(); ++i)
    {
        if(!marked[i])
        {
            newpos.Add(pos[i]);
        }
    }
    return newpos;
}

TArray<ULineV2*> ULineFunction::ConnectNearestLines(const TArray<ULineEnd*>& les, TArray<ULineV2*> pos, float d2, float angle)
{
    d2 *= d2;
    for(int32 i = 0; i < les.Num(); ++i)
    {
        const ULineEnd& le1 = *les[i];
        int32 minbegp = -1, minbeg_angle = angle, minbeg_dis = d2, minbeg_id = -1;
        int32 minendp = -1, minend_angle = angle, minend_dis = d2, minend_id = -1;
        FVector2D begdst, enddst;
        for(int32 j = 0; j < pos.Num(); ++j)
        {
            TArray<FVector2D>& nowLine = pos[j]->pts;
            if(le1.idx == j)
            {
                continue;
            }
            for(int32 k = 0; k < nowLine.Num(); k++)
            {
                float dis1 = FVector2D::DistSquared(le1.beg, nowLine[k]);
                float dis2 = FVector2D::DistSquared(le1.end, nowLine[k]);
                if(dis1 < minbeg_dis && !le1.haslink1)
                {
                    FVector2D p1 = nowLine[k] - le1.beg;
                    float angle1 = atan2(p1.X, p1.Y) * M_1_PI * 180;
                    angle1 = std::min(abs(le1.angleBeg - angle1), abs(le1.angleBeg - (angle1 + 360)));
                    if(angle1 < angle)
                    {
                        minbeg_angle = angle1;
                        minbeg_dis = dis1;
                        minbegp = k;
                        begdst = nowLine[minbegp];
                        minbeg_id = j;
                    }
                }
                if(dis2 < minend_dis && !le1.haslink2)
                {
                    FVector2D p1 = nowLine[k] - le1.end;
                    float angle1 = atan2(p1.X, p1.Y) * M_1_PI * 180;
                    angle1 = std::min(abs(le1.angleEnd - angle1), abs(le1.angleEnd - (angle1 + 360)));
                    if(angle1 < angle)
                    {
                        minend_angle = angle1;
                        minend_dis = dis2;
                        minendp = k;
                        enddst = nowLine[minendp];
                        minend_id = j;
                    }
                }
            }
        }
        const int32 ELONGATION_LEN = 15;
        if(minbegp != -1 && FVector2D::DistSquared(pos[le1.idx]->pts[0], pos[le1.idx]->pts.Last()) > 1)
        {
            const TArray<FVector2D>& orgin = pos[minbeg_id]->pts;
            TArray<FVector2D>& elongation = pos[le1.idx]->pts;
            const TArray<FVector2D> normals = GetNormalsLen2(orgin);
            elongation.Insert(begdst, 0);
            int32 orgin_id1 = minbegp - 2;
            int32 orgin_id2 = minbegp + 2;
            if(orgin_id1 >= 0 && orgin_id2 < orgin.Num())
            {
                FVector2D n1 = orgin[minbegp] + normals[minbegp] * 2;
                FVector2D n2 = orgin[minbegp] - normals[minbegp] * 2;
                FVector2D vec1 = orgin[orgin_id1] - orgin[orgin_id2];
                FVector2D vec2 = elongation[0] - elongation[1];
                vec1.Normalize();
                vec2.Normalize();
                float side_angle1 = acos(FVector2D::DotProduct(vec1, vec2)) * 180 * M_1_PI;
                float side_angle2 = acos(FVector2D::DotProduct(vec1, -vec2)) * 180 * M_1_PI;
                if(side_angle1 < 45 || side_angle2 < 45)
                {
                    float dis1 = FVector2D::DistSquared(orgin[orgin_id1], elongation[0] + vec2 * 2);
                    float dis2 = FVector2D::DistSquared(orgin[orgin_id2], elongation[0] + vec2 * 2);
                    float ndis1 = FVector2D::DistSquared(n1, elongation[1]);
                    float ndis2 = FVector2D::DistSquared(n2, elongation[1]);
                    float nlen = FVector2D::Distance(orgin[minbegp], elongation[1]);
                    for(int32 h = minbegp - 20; h < minbegp + 20 && h < orgin.Num(); ++h)
                    {
                        if(h < 0)
                        {
                            h = 0;
                        }
                        float tlen = FVector2D::Distance(orgin[h], elongation[1]);
                        if(tlen < nlen)
                        {
                            nlen = tlen;
                        }
                    }
                    nlen *= 0.6;
                    if(dis1 < dis2 && orgin_id1 < ELONGATION_LEN)
                    {
                        elongation.RemoveAt(0);
                        if(ndis1 < ndis2)
                        {
                            for(int32 i = minbegp; i >= 0; --i)
                            {
                                elongation.Insert(orgin[i] + normals[i] * nlen * (i / (float)minbegp), 0);
                            }
                        }
                        else
                        {
                            for(int32 i = minbegp; i >= 0; --i)
                            {
                                elongation.Insert(orgin[i] - normals[i] * nlen * (i / (float)minbegp), 0);
                            }
                        }
                        elongation.Insert(orgin[0], 0);
                    }
                    else if(dis1 < dis2 && (orgin.Num() - orgin_id1) < ELONGATION_LEN)
                    {
                        elongation.RemoveAt(0);
                        int32 div = orgin.Num() - minbegp;
                        if(ndis1 < ndis2)
                        {
                            for(int32 i = minbegp; i < orgin.Num(); ++i)
                            {
                                elongation.Insert(orgin[i] + normals[i] * nlen * ((orgin.Num() - i) / (float)div), 0);
                            }
                        }
                        else
                        {
                            for(int32 i = minbegp; i < orgin.Num(); ++i)
                            {
                                elongation.Insert(orgin[i] - normals[i] * nlen * ((orgin.Num() - i) / (float)div), 0);
                            }
                        }
                        elongation.Insert(orgin.Last(), 0);
                    }
                }
            }
        }
        if(minendp != -1 && FVector2D::Distance(pos[le1.idx]->pts[0], pos[le1.idx]->pts.Last()) > 1)
        {
            TArray<FVector2D>& orgin = pos[minend_id]->pts;
            TArray<FVector2D>& elongation = pos[le1.idx]->pts;
            TArray<FVector2D> normals = GetNormalsLen2(orgin);
            elongation.Add(enddst);
            int32 orgin_id1 = minendp - 2;
            int32 orgin_id2 = minendp + 2;
            if(orgin_id1 >= 0 && orgin_id2 < orgin.Num())
            {
                FVector2D n1 = orgin[minendp] + normals[minendp] * 2;
                FVector2D n2 = orgin[minendp] - normals[minendp] * 2;
                int32 last = elongation.Num() - 1;
                FVector2D vec1 = orgin[orgin_id1] - orgin[orgin_id2];
                FVector2D vec2 = elongation[last] - elongation[last - 1];
                vec1.Normalize();
                vec2.Normalize();
                float side_angle1 = acos(FVector2D::DotProduct(vec1, vec2)) * 180 * M_1_PI;
                float side_angle2 = acos(FVector2D::DotProduct(vec1, -vec2)) * 180 * M_1_PI;
                if(side_angle1 < 45 || side_angle2 < 45)
                {
                    float dis1 = FVector2D::DistSquared(orgin[orgin_id1], elongation[last] + vec2 * 2);
                    float dis2 = FVector2D::DistSquared(orgin[orgin_id2], elongation[last] + vec2 * 2);
                    float ndis1 = FVector2D::DistSquared(n1, elongation[last - 1]);
                    float ndis2 = FVector2D::DistSquared(n2, elongation[last - 1]);
                    float nlen = FVector2D::Distance(orgin[minendp], elongation[last - 1]);
                    for(int32 h = minendp - 20; h < minendp + 20 && h < orgin.Num(); ++h)
                    {
                        if(h < 0)
                        {
                            h = 0;
                        }
                        float tlen = FVector2D::Distance(orgin[h], elongation[last - 1]);
                        if(tlen < nlen)
                        {
                            nlen = tlen;
                        }
                    }
                    nlen *= 0.6;
                    if(dis1 < dis2 && orgin_id1 < ELONGATION_LEN)
                    {
                        elongation.RemoveAt(elongation.Num() - 1);
                        if(ndis1 < ndis2)
                        {
                            for(int32 i = minendp; i >= 0; --i)
                            {
                                elongation.Add(orgin[i] + normals[i] * nlen * (i / (float)minendp));
                            }
                        }
                        else
                        {
                            for(int32 i = minendp; i >= 0; --i)
                            {
                                elongation.Add(orgin[i] - normals[i] * nlen * (i / (float)minendp));
                            }
                        }
                        elongation.Add(orgin[0]);
                    }
                    else if(dis1 > dis2 && (orgin.Num() - orgin_id1) < ELONGATION_LEN)
                    {
                        elongation.RemoveAt(elongation.Num() - 1);
                        int32 div = orgin.Num() - minendp;
                        if(ndis1 < ndis2)
                        {
                            for(int32 i = minendp; i < orgin.Num(); ++i)
                            {
                                elongation.Add(orgin[i] + normals[i] * nlen * ((orgin.Num() - i) / (float)div));
                            }
                        }
                        else
                        {
                            for(int32 i = minendp; i < orgin.Num(); ++i)
                            {
                                elongation.Add(orgin[i] - normals[i] * nlen * ((orgin.Num() - i) / (float)div));
                            }
                        }
                        elongation.Add(orgin.Last());
                    }
                }
            }
        }
    }
    return pos;
}

FVector2D ULineFunction::GetRotation(const FVector2D& src, float angle, const FVector2D& middle)
{

    FVector tmpsrc(src.X, src.Y, 0);
    FVector up(middle.X, middle.Y, -1);
    FQuat q(up, angle / 180.f * M_PI);
    tmpsrc = q * tmpsrc;
    return FVector2D(tmpsrc.X, tmpsrc.Y);
}

TArray<ULineEnd*> ULineFunction::GetLineEnds(const TArray<ULineV2*>& cvp)
{
    TArray<ULineEnd*> ans;
    for(int32 i = 0; i < cvp.Num(); ++i)
    {
        const TArray<FVector2D>& nowLine = cvp[i]->pts;
        int32 last = nowLine.Num() - 1;
        FVector2D p1 = nowLine[0] - nowLine[2];
        FVector2D p2 = nowLine[last] - nowLine[last - 2];
        if(nowLine.Num() > 15)
        {
            p1 = nowLine[5] - nowLine[10];
            p2 = nowLine[last - 5] - nowLine[last - 10];
        }
        float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
        float angle2 = fmod(360 + atan2(p2.X, p2.Y) * M_1_PI * 180, 360);
        ans.Add(NewObject<ULineEnd>(new ULineEnd(nowLine[0], nowLine.Last(), i, last, angle1, angle2)));
    }
    return ans;
}



TArray<FVector2D> ULineFunction::SmoothingEach5(const TArray<FVector2D>& cvp, float centroidRadio /*= 1.0f*/, int32 repeat /*= 1*/)
{
    if(cvp.Num() <= 2)
    {
        TArray<FVector2D> res = cvp;
        return res;
    }
    TArray<FVector2D> cps = cvp;
    TArray<FVector2D> newcps;
    TArray<FVector2D> centroids;
    for(int32 repeatCount = 0; repeatCount < repeat; repeatCount++)
    {
        newcps.Reset();
        if(cps.Num() <= 5)
        {
            newcps.Add(cps[0]);
            for(int32 j = 1; j < cps.Num() - 1; j++)
            {
                auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1]) * 0.25;
                newcps.Add(vec);
            }
            newcps.Add(cps.Last());
            cps = newcps;
        }
        else
        {
            centroids.Reset();
            newcps.Add(cps[0]);
            centroids.Add((cps[0] + cps[1] + cps[2]) / 3.0f);
            newcps.Add((cps[0] + cps[1] * 2 + cps[2]) * 0.25);
            for(int32 j = 2; j < cps.Num() - 2; j++)
            {
                auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1] + cps[j + 2] + cps[j - 2]) /
                           6.0f;
                newcps.Add(vec);
                centroids.Add((cps[j] + cps[j + 1] + cps[j - 1] + cps[j + 2] +
                               cps[j - 2]) / 5.0f);
            }
            int32 last = (int)cps.Num() - 1;
            newcps.Add((cps[last] + cps[last - 1] * 2 + cps[last - 2]) * 0.25);
            centroids.Add((cps[last] + cps[last - 1] + cps[last - 2]) / 3.0f);
            newcps.Add(cps.Last());
            cps = newcps;
            // move centroid
            newcps.Reset();
            newcps.Add(cps[0]);
            FVector2D cert = (cps[0] + cps[1] + cps[2]) / 3.0f;
            cert = centroids[0] - cert;
            newcps.Add(cps[1] + cert);
            for(int32 j = 2; j < (int)cps.Num() - 2; j++)
            {
                FVector2D nowCentroid = (cps[j] + cps[j + 1] + cps[j - 1] + cps[j + 2] +
                                         cps[j - 2]) / 5.0f;
                nowCentroid = centroids[j - 1] - nowCentroid;
                newcps.Add(cps[j] + nowCentroid * centroidRadio);
            }
            cert = (cps[last] + cps[last - 1] + cps[last - 2]) / 3.0f;
            cert = centroids[last - 2] - cert;
            newcps.Add(cps[last - 1] + cert);
            newcps.Add(cps.Last());
            cps = newcps;
        }
    }
    return cps;
}

ULineV2* ULineFunction::SmoothingEach5_ueLine(const ULineV2* ul, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
    ULineV2* res = NewObject<ULineV2>();
    res->pts = SmoothingEach5(ul->pts);
    return res;
}

TArray<ULineV2*> ULineFunction::SmoothingEach5_Array(const TArray<ULineV2*>& cvp, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
    TArray<ULineV2*> ans;
	ans.SetNumZeroed(cvp.Num());
    for(int32 i = 0; i < cvp.Num(); ++i)
    {
        const ULineV2* aa = cvp[i];
        ans[i] = SmoothingEach5_ueLine(aa, centroidRadio, repeat);
    }
    return ans;
}

TArray<FVector2D> ULineFunction::SmoothingEach3(const TArray<FVector2D>& cvp, float centroidRadio /*= 1.0f*/, int32 repeat /*= 1*/)
{
    if(cvp.Num() <= 2)
    {
        TArray<FVector2D> res = cvp;
        return res;
    }

    TArray<FVector2D> cps = cvp;
    TArray<FVector2D> newcps;
    for(int32 repeatCount = 0; repeatCount < repeat; repeatCount++)
    {
        newcps.Reset();
        if(cps.Num() <= 5)
        {
            newcps.Add(cps[0]);
            for(int32 j = 1; j < cps.Num() - 1; j++)
            {
                auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1]) * 0.25;
                newcps.Add(vec);
            }
            newcps.Add(cps.Last());
            cps = newcps;
        }
        else
        {
            newcps.Add(cps[0] * 0.8);
            for(int32 j = 2; j < cps.Num() - 2; j++)
            {
                if(cps[j].X > 0)
                {
                    int32 zero = 0;
                    for(int32 k = j - 1; k <= j + 1; ++k)
                    {
                        if(cps[k].X < 0.01)
                        {
                            zero++;
                        }
                    }
                    auto vec = (cps[j] * (2 + zero) + cps[j + 1] + cps[j - 1]) / 4.0f;
                    newcps.Add(vec);
                }
                else
                {
                    newcps.Add(FVector2D());
                }
            }
            newcps.Add(cps.Last() * 0.8);
            cps = newcps;
        }
    }
    return cps;
}

ULineV2* ULineFunction::SmoothingEach3_ueLine(const ULineV2* ul, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
    ULineV2* res = NewObject<ULineV2>();
    res->pts = SmoothingEach3(ul->pts);
    return res;
}

TArray<ULineV2*> ULineFunction::SmoothingEach3_Array(const TArray<ULineV2*>& cvp, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
    TArray<ULineV2*> ans;
	ans.SetNumZeroed(cvp.Num());
    for(int32 i = 0; i < cvp.Num(); ++i)
    {
        const ULineV2* aa = cvp[i];
        ans[i] = SmoothingEach3_ueLine(aa, centroidRadio, repeat);
    }
    return ans;
}


TArray<FVector2D> ULineFunction::GetNormalsLen2(TArray<FVector2D> cvp)
{
    if(cvp.Num() < 2)
    {
        TArray<FVector2D> tmp;
		tmp.SetNumZeroed(cvp.Num());
        return tmp;
    }
    TArray<FVector2D> normals;
    normals.Add(GetRotation(cvp[1] - cvp[0], -90));
    for(int32 i = 1; i < cvp.Num(); ++i)
    {
        normals.Add(GetRotation(cvp[i] - cvp[i - 1],
                                -90));
    }
    for(int32 i = 0; i < normals.Num(); ++i)
    {
        normals[i].Normalize();
    }
    return normals;
}

TArray<ULineV2*> ULineFunction::GetNormalsLen2_Array(TArray<ULineV2*> cvp)
{
    TArray<ULineV2*> ans;
	ans.SetNumZeroed(cvp.Num());
    for(int32 i = 0; i < cvp.Num(); ++i)
    {
        const ULineV2* aa = cvp[i];
        ans[i] = GetNormalsLen2_ueLine(aa);
    }
    return ans;
}

ULineV2* ULineFunction::GetNormalsLen2_ueLine(const ULineV2* ul)
{
    ULineV2* res = NewObject<ULineV2>();
    res->pts = GetNormalsLen2(ul->pts);
    return res;
}

TArray<float> ULineFunction::ConvertToSquareWave(const TArray<float>& data, int32 error, float val, float zero)
{
    floats ans;
    const int32 size = data.Num();
    for(int32 i = 0; i < size; ++i)
    {
        if(data[i] - zero > error)
        {
            ans.Push(zero + val);
        }
        else if(zero - data[i] > error)
        {
            ans.Push(zero - val);
        }
        else
        {
            ans.Push(zero);
        }
    }
    return ans;
}

TArray<float> ULineFunction::ConvertToAngle(const TArray<float>& data, float zero /*= 290*/)
{
    zero -= 90;
    floats tmp = data;
    floats ans;
    {
        float dy = tmp[2] - tmp[0];
        float angle = atan2(3, dy) * M_1_PI * 180 + zero;
        ans.Push(angle);
        dy = tmp[3] - tmp[1];
        angle = atan2(3, dy) * M_1_PI * 180 + zero;
        ans.Push(angle);
    }
    for(int32 i = 2; i < data.Num() - 2; ++i)
    {
        float dy = tmp[i + 2] - tmp[i - 2];
        float angle = atan2(3, dy) * M_1_PI * 180 + zero;
        ans.Push(angle);
    }
    {
        int32 last = (int)data.Num() - 2;
        float dy = tmp[last - 1] - tmp[last - 3];
        float angle = atan2(3, dy) * M_1_PI * 180 + zero;
        ans.Push(angle);
        dy = tmp[last] - tmp[last - 1];
        angle = atan2(3, dy) * M_1_PI * 180 + zero;
        ans.Push(angle);
    }
    return ans;
}

TArray<ULineV2*> ULineFunction::BuildDecorativeCurve(const TArray<ULineV2*>& blackLine, UcvMat* umat, float blackRadio)
{
	ULines res;
	ULines normals = GetNormalsLen2_Array(blackLine);
	res.SetNumZeroed(blackLine.Num());
    for(int32 i = 0; i < blackLine.Num(); ++i)
    {
        res[i] = NewObject<ULineV2>();
    }
    for(int32 idx1 = 0; idx1 < blackLine.Num(); ++idx1)
    {
        const FVector2Ds& nowLine = blackLine[idx1]->pts;
        const FVector2Ds& nowNormals = normals[idx1]->pts;
        TArray<FVector2D>& lineWidths = res[idx1]->pts;
        lineWidths.Reset();
        for(int32 idx2 = 0; idx2 < nowLine.Num() - 1; ++idx2)
        {
            const float LINE_WIDTH = 5;
            FVector2D start(nowLine[idx2] - nowNormals[idx2] * LINE_WIDTH);
            FVector2D end(nowLine[idx2] + nowNormals[idx2] * LINE_WIDTH);
            FVector2D start2(nowLine[idx2 + 1] - nowNormals[idx2 + 1] * LINE_WIDTH);
            FVector2D end2(nowLine[idx2 + 1] + nowNormals[idx2 + 1] * LINE_WIDTH);
            floats line1 = umat->GetLineLight(start.X, start.Y, end.X, end.Y, 360);
            floats line2 = umat->GetLineLight(start2.X, start2.Y, end2.X, end2.Y, 360);
            floats width1 = ConvertToLineWidth(ConvertToSquareWave(ConvertToAngle(line1),
                                               15, 50), LINE_WIDTH * 2);
            floats width2 = ConvertToLineWidth(ConvertToSquareWave(ConvertToAngle(line2),
                                               15, 50), LINE_WIDTH * 2);
			if (width1.Num() >= 2 && width2.Num() >= 2 && abs(width2[0] - width2[1]) < 1.5)
            {
				FVector2Ds line1;
                line1.Push(nowLine[idx2] - nowNormals[idx2] * width1[0] * blackRadio);
                line1.Push(nowLine[idx2 + 1] - nowNormals[idx2 + 1] * width2[0] *
                           blackRadio);
				line1 = ULineV2::GetLine_FV2Array(line1, 0.5, 0.5);
				FVector2Ds line2;
                line2.Push(nowLine[idx2] + nowNormals[idx2] * width1[1] * blackRadio);
                line2.Push(nowLine[idx2 + 1] + nowNormals[idx2 + 1] * width2[1] *
                           blackRadio);
				line2 = ULineV2::GetLine_FV2Array(line2, 0.5, 0.5);
                // save line width
				lineWidths.Push(FVector2D(width1[0] * blackRadio, width1[1] * blackRadio));
            }
            else
            {
				lineWidths.Push(FVector2D());
            }
        }
		lineWidths.Push(FVector2D());
    }
	return res;
}

TArray<float> ULineFunction::ConvertToLineWidth(const TArray<float>& data, float lineWidth, float zero /*= 290*/)
{
    floats ans;
    bool end = false;
    const int32 check_length = 179;
    const int32 size = data.Num();
    for(int32 i = size / 2; i > size / 2 - check_length && !end; --i)
    {
        if(data[i] > zero)
        {
            for(int32 j = i; j >= 0; --j)
            {
                if(data[j] <= zero)
                {
                    ans.Push(lineWidth * 0.5 - j * lineWidth / 360.0);
                    end = true;
                    break;
                }
                else if(j == 0)
                {
                    ans.Push(lineWidth * 0.5 - j * lineWidth / 360.0);
                    end = true;
                }
            }
        }
    }
    for(int32 i = size / 2; i < size / 2 + check_length; ++i)
    {
        if(data[i] < zero)
        {
            for(int32 j = i; j < size; ++j)
            {
                if(data[j] >= zero)
                {
                    ans.Push(j * lineWidth / 360.0 - lineWidth * 0.5);
                    return ans;
                }
                else if(j == size - 1)
                {
                    ans.Push(j * lineWidth / 360.0 - lineWidth * 0.5);
                    return ans;
                }
            }
        }
    }
    return floats();
}

void ULineFunction::ClearLineWidthByPercent(TArray<ULineV2*>& widths, float angle)
{
	for (int32 i = 0; i < widths.Num(); ++i)
	{
		FVector2Ds& nowWidths = widths[i]->pts;
		double p = GetLineWidthPercent(nowWidths);
		if (p < angle)
		{
			nowWidths.SetNumZeroed(widths.Num());
		}
		else
		{
			//nowWidths = FixLineWidths(nowWidths, 200);
		}
	}
}

float ULineFunction::GetLineWidthPercent(const TArray<FVector2D>& cvp)
{
	float n = 0;
	for (int32 i = 0; i < cvp.Num(); ++i)
	{
		if (cvp[i].X > 0.1)
		{
			n += 1;
		}
		else if (cvp[i].Y > 0.1)
		{
			n += 1;
		}
	}
	return n / cvp.Num();
}

ULineV2* ULineFunction::FixWidthLine_ueLine(const ULineV2* uline, int32 range)
{
	ULineV2* res = NewObject<ULineV2>();
	res->pts = FixWidthLine(uline->pts, range);
	return res;
}

TArray<FVector2D> ULineFunction::FixWidthLine(const TArray<FVector2D>& cvp, int32 range)
{
	if (cvp.Num() <= range * 2 + 1)
	{
		FVector2Ds res = cvp;
		return res;
	}
	FVector2Ds cps = cvp;
	FVector2Ds newcps;
	newcps.Insert(&cps[0], range, 0);
	FVector2D sum;
	int cc = 0;
	for (int j = range; j < cps.Num() - range; j++)
	{
		cc++;
		sum += cps[j];
	}
	sum /= cc;
	for (int j = range; j < cps.Num() - range; j++)
	{
		newcps.Push(sum);
	}
	newcps.Append(&cps[cps.Num() - range], range);
	cps = newcps;
	return cps;
}

TArray<ULineV2*> ULineFunction::FixWidthLine_Array(const TArray<ULineV2*>& cvp, int32 range)
{
	TArray<ULineV2*> res;
	res.SetNumZeroed(cvp.Num());
	for (int32 i = 0; i < cvp.Num(); ++i)
	{
		res[i] = FixWidthLine_ueLine(cvp[i], range);
	}
	return res;
}

TArray<FVector2D> ULineFunction::SmoothingEach5Skip0(const TArray<FVector2D>& cvp, double centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
	TArray<FVector2D> cps = cvp;
	if (cvp.Num() <= 2)
	{
		return cps;
	}
	
	TArray<FVector2D> newcps;
	for (int repeatCount = 0; repeatCount < repeat; repeatCount++)
	{
		newcps.Reset();
		if (cps.Num() <= 5)
		{
			newcps.Push(cps[0]);
			for (int j = 1; j < cps.Num() - 1; j++)
			{
				auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1]) * 0.25;
				newcps.Push(vec);
			}
			newcps.Push(cps.Last());
			cps = newcps;
		}
		else
		{
			newcps.Push(cps[0] * 0.8);
			newcps.Push((cps[0] + cps[1] * 2 + cps[2]) * 0.25);
			for (int j = 2; j < cps.Num() - 2; j++)
			{
				if (cps[j].X > 0)
				{
					int zero = 0;
					for (int k = j - 2; k <= j + 2; ++k)
					{
						if (cps[k].X < 0.01)
						{
							zero++;
						}
					}
					FVector2D vec = (cps[j] * (2 + zero) + cps[j + 1] + cps[j - 1] + cps[j + 2] +
						cps[j - 2]) / 6.0f;
					newcps.Push(vec);
				}
				else
				{
					newcps.Push(FVector2D());
				}
			}
			int last = cps.Num() - 1;
			newcps.Push((cps[last] + cps[last - 1] * 2 + cps[last - 2]) * 0.25);
			newcps.Push(cps.Last() * 0.8);
			cps = newcps;
		}
	}
	return cps;
}

ULineV3* ULineFunction::GetLinesColor(UcvMat* img, const ULineV2* lines)
{
	ULineV3* res = NewObject<ULineV3>();

	return res;
}

