// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineFunction.h"
#include "ueLine.h"
#include "LineEnd.h"
#include <cmath>
#include <vector>

TArray<ULineEnd*> ULineFunction::LinkLineEnds180(TArray<ULineEnd*> les, float distance, float angle)
{
	float sdistance = distance * distance;
	for (int32 i = 0; i < les.Num(); ++i)
	{
		ULineEnd& le1 = *les[i];
		for (int32 j = 0; j < les.Num(); ++j)
		{
			if (i != j)
			{
				bool haslink = false;
				ULineEnd& le2 = *les[j];
				// find begin link
				if (FVector2D::DistSquared(le1.beg, le2.beg) < sdistance)
				{
					if (CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::BEGIN_TO_BEGIN, angle))
					{
						le1.beglinks.Add(j);
						le2.beglinks.Add(i);
						le1.beglink = &le2;
						le2.beglink = &le1;
						haslink = true;
					}
				}
				else if (FVector2D::DistSquared(le1.beg, le2.end) < sdistance)
				{
					if (CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::BEGIN_TO_END, angle))
					{
						le1.beglinks.Add(j);
						le2.endlinks.Add(i);
						le1.beglink = &le2;
						le2.endlink = &le1;
						haslink = true;
					}
				}
				// find end link
				if (FVector2D::DistSquared(le1.end, le2.beg) < sdistance)
				{
					if (CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::END_TO_BEGIN, angle))
					{
						le1.endlinks.Add(j);
						le2.beglinks.Add(i);
						le1.endlink = &le2;
						le2.beglink = &le1;
						haslink = true;
					}
				}
				else if (FVector2D::DistSquared(le1.end, le2.end) < sdistance)
				{
					if (CheckLinkEnd_Similarity180(&le1, &le2, ELinkMethod::END_TO_END, angle))
					{
						le1.endlinks.Add(j);
						le2.endlinks.Add(i);
						le1.endlink = &le2;
						le2.endlink = &le1;
						haslink = true;
					}
				}
				if (haslink)
				{
					if (le1.linkIdx1)
					{
						le1.linkIdx2 = &le2;
					}
					else
					{
						le1.linkIdx1 = &le2;
					}
					if (le2.linkIdx1)
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
	switch (c)
	{
	case ELinkMethod::BEGIN_TO_BEGIN:
		if (CheckAngle(lhs.angleBeg, rangleBeg, angle))
		{
			FVector2D p1 = rhs.beg - lhs.beg;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (CheckAngle(lhs.angleBeg, angle1, angle) &&
				CheckAngle(rangleBeg, angle1, angle))
			{
				return true;
			}
		}
		break;
	case ELinkMethod::BEGIN_TO_END:
		if (CheckAngle(lhs.angleBeg, rangleEnd, angle))
		{
			FVector2D p1 = rhs.end - lhs.beg;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (CheckAngle(lhs.angleBeg, angle1, angle) &&
				CheckAngle(rangleEnd, angle1, angle))
			{
				return true;
			}
		}
		break;
	case ELinkMethod::END_TO_BEGIN:
		if (CheckAngle(lhs.angleEnd, rangleBeg, angle))
		{
			FVector2D p1 = rhs.beg - lhs.end;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (CheckAngle(lhs.angleEnd, angle1, angle) &&
				CheckAngle(rangleBeg, angle1, angle))
			{
				return true;
			}
		}
		break;
	case ELinkMethod::END_TO_END:
		if (CheckAngle(lhs.angleEnd, rangleEnd, angle))
		{
			FVector2D p1 = rhs.end - lhs.end;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (CheckAngle(lhs.angleEnd, angle1, angle) &&
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
	switch (c)
	{
	case ELinkMethod::BEGIN_TO_BEGIN:
		if (abs(lhs.angleBeg - rangleBeg) < angle || abs(lhs.angleBeg - rangleBeg2) < angle)
		{
			FVector2D p1 = rhs.beg - lhs.beg;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (abs(lhs.angleBeg - angle1) < angle)
			{
				return true;
			}
		}
		break;
	case ELinkMethod::BEGIN_TO_END:
		if (abs(lhs.angleBeg - rangleEnd) < angle || abs(lhs.angleBeg - rangleEnd2) < angle)
		{
			FVector2D p1 = rhs.end - lhs.beg;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (abs(lhs.angleBeg - angle1) < angle)
			{
				return true;
			}
		}
		break;
	case ELinkMethod::END_TO_BEGIN:
		if (abs(lhs.angleEnd - rangleBeg) < angle || abs(lhs.angleEnd - rangleBeg2) < angle)
		{
			FVector2D p1 = rhs.beg - lhs.end;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (abs(lhs.angleEnd - angle1) < angle)
			{
				return true;
			}
		}
		break;
	case ELinkMethod::END_TO_END:
		if (abs(lhs.angleEnd - rangleEnd) < angle || abs(lhs.angleEnd - rangleEnd2) < angle)
		{
			FVector2D p1 = rhs.end - lhs.end;
			float angle1 = fmod(360 + atan2(p1.X, p1.Y) * M_1_PI * 180, 360);
			if (abs(lhs.angleEnd - angle1) < angle)
			{
				return true;
			}
		}
		break;
	}
	return false;
}

TArray<UueLine*> ULineFunction::ConnectLineEnds3(const TArray<ULineEnd*>& les, TArray<UueLine*> pos)
{
	std::vector<bool> marked(pos.Num(), false);
	TArray<UueLine*> newpos;
	for (auto it1 = les.CreateConstIterator(); it1; ++it1)
	{
		const ULineEnd& le1 = *(*it1);
		if (le1.beglinks.Num()>0)
		{
			int nowid = le1.idx;
			if (marked[nowid])
			{
				continue;
			}
			UueLine* uaddline = NewObject<UueLine>();
			TArray<FVector2D>& addline = uaddline->pts;
			addline.Append(pos[nowid]->pts);
			marked[nowid] = true;
			nowid = le1.beglinks[0];
			while (nowid != -1 && !marked[nowid])
			{
				marked[nowid] = true;
				int next = nowid;
				const UueLine* loopline = pos[nowid];
				float dis1 = FVector2D::Distance(loopline->pts[0], addline.Last());
				float dis2 = FVector2D::Distance(loopline->pts.Last(), addline.Last());
				if (dis1 < dis2)
				{
					addline.Append(loopline->pts);
					if (les[nowid]->endlinks.Num() > 0)
					{
						next = les[nowid]->endlinks.Last();
					}
				}
				else
				{
// 					TArray<FVector2D> reverseline;
// 					reverseline.Reserve(loopline->pts.Num());
// 					for (int32 i = loopline->pts.Num() - 1; i >= 0; --i)
// 					{
// 						reverseline.Add(loopline->pts[i]);
// 					}
					std::reverse((FVector2D*)(loopline->pts.GetData()), 
						(FVector2D*)(loopline->pts.GetData() + loopline->pts.Num()));
					addline.Append(loopline->pts);
					if (les[nowid]->beglinks.Num() > 0)
					{
						next = les[nowid]->beglinks[0];
					}
				}
				if (next == nowid)
				{
					nowid = -1;
				}
			}
			newpos.Add(uaddline);
		}
		else if (le1.endlinks.Num() > 0)
		{
			int nowid = le1.idx;
			if (marked[nowid])
			{
				continue;
			}
			UueLine* uaddline = NewObject<UueLine>();
			TArray<FVector2D>& addline = uaddline->pts;
			addline.Append(pos[nowid]->pts);
			marked[nowid] = true;
			nowid = le1.endlinks[0];
			while (nowid != -1 && !marked[nowid])
			{
				marked[nowid] = true;
				int next = nowid;
				const UueLine* loopline = pos[nowid];
				double dis1 = FVector2D::Distance(loopline->pts[0], addline.Last());
				double dis2 = FVector2D::Distance(loopline->pts.Last(), addline.Last());
				if (dis1 < dis2)
				{
					addline.Append(loopline->pts);
					if (les[nowid]->endlinks.Num() > 0)
					{
						next = les[nowid]->endlinks.Last();
					}
				}
				else
				{
// 					TArray<FVector2D> reverseline;
// 					reverseline.Reserve(loopline->pts.Num());
// 					for (int32 i = loopline->pts.Num() - 1; i >= 0; --i)
// 					{
// 						reverseline.Add(loopline->pts[i]);
// 					}
					std::reverse((FVector2D*)(loopline->pts.GetData()),
						(FVector2D*)(loopline->pts.GetData() + loopline->pts.Num()));
					addline.Append(loopline->pts);
					if (les[nowid]->beglinks.Num() > 0)
					{
						next = les[nowid]->beglinks[0];
					}
				}
				if (next == nowid)
				{
					nowid = -1;
				}
			}
			newpos.Add(uaddline);
		}
	}
	for (int32 i = 0; i < les.Num(); ++i)
	{
		if (!marked[i])
		{
			newpos.Add(pos[i]);
		}
	}
	return newpos;
}

