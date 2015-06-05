// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineV2.h"

void ULineV2::Move(FVector2D vec)
{
	for (int32 i = 0; i < pts.Num(); ++i)
	{
		pts[i] += vec;
	}
}

ULineV2* ULineV2::Clone()
{
	ULineV2* res = NewObject<ULineV2>();
	res->pts = this->pts;
	return res;
}

ULineV2* ULineV2::GetLine(const CvLine& line, float mx, float my)
{
	ULineV2* res = NewObject<ULineV2>();
	for (CvLine::const_iterator it = line.begin(); it != line.end(); ++it)
	{
		res->pts.Add(FVector2D(it->x + mx, it->y + my));
	}
	return res;
}

ULineV2* ULineV2::GetLine_Array(TArray<FVector2D> line)
{
	ULineV2* res = NewObject<ULineV2>();
	res->pts = line;
	return res;
}


TArray<ULineV2*> ULineV2::GetLines(const CvLines& lines, float mx, float my)
{
	TArray<ULineV2*> res;
	for (int32 i = 0; i < lines.size(); ++i)
	{
		res.Push(GetLine(lines[i]));
	}
	return res;
}

TArray<FVector2D> ULineV2::GetLine_FV2Array(TArray<FVector2D> line, float mx, float my)
{
	TArray<FVector2D> res;
	res.SetNum(line.Num());
	for (int32 i = 0; i < line.Num(); ++i)
	{
		res[i] = line[i] + FVector2D(mx, my);
	}
	return res;
}

TArray<ULineV2*> ULineV2::CloneArray(const TArray<ULineV2*>& src)
{
	TArray<ULineV2*> res;
	res.Reset(src.Num());
	for (int32 i = 0; i < src.Num(); ++i)
	{
		res[i] = src[i]->Clone();
	}
	return res;
}

