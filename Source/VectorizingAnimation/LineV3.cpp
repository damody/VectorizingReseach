// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineV3.h"

void ULineV3::Move(FVector2D vec)
{
	for (int i = 0; i < pts.Num(); ++i)
	{
		pts[i] += vec;
	}
}

ULineV3* ULineV3::Clone()
{
	ULineV3* res = NewObject<ULineV3>();
	res->pts = this->pts;
	return res;
}

ULineV3* ULineV3::GetLine(const CvLine& line, float mx, float my)
{
	ULineV3* res = NewObject<ULineV3>();
	for (CvLine::const_iterator it = line.begin(); it != line.end(); ++it)
	{
		res->pts.Add(FVector2D(it->x + mx, it->y + my));
	}
	return res;
}

ULineV3* ULineV3::GetLine_Array(TArray<FVector2D> line)
{
	ULineV3* res = NewObject<ULineV3>();
	res->pts = line;
	return res;
}


TArray<ULineV3*> ULineV3::GetLines(const CvLines& lines, float mx, float my)
{
	TArray<ULineV3*> res;
	for (int i = 0; i < lines.size(); ++i)
	{
		res.Push(GetLine(lines[i]));
	}
	return res;
}

TArray<FVector2D> ULineV3::GetLine_FV2Array(TArray<FVector2D> line, float mx, float my)
{
	TArray<FVector2D> res;
	res.SetNum(line.Num());
	for (int32 i = 0; i < line.Num(); ++i)
	{
		res[i] = line[i] + FVector2D(mx, my);
	}
	return res;
}

