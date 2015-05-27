// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ueLine.h"

void UueLine::Move(FVector2D vec)
{
	for (int i = 0; i < pts.Num(); ++i)
	{
		pts[i] += vec;
	}
}

UueLine* UueLine::Clone()
{
	UueLine* res = NewObject<UueLine>();
	res->pts = this->pts;
	return res;
}

UueLine* UueLine::GetLine(const CvLine& line, float mx, float my)
{
	UueLine* res = NewObject<UueLine>();
	for (CvLine::const_iterator it = line.begin(); it != line.end(); ++it)
	{
		res->pts.Add(FVector2D(it->x + mx, it->y + my));
	}
	return res;
}

UueLine* UueLine::GetLine_Array(TArray<FVector2D> line)
{
	UueLine* res = NewObject<UueLine>();
	res->pts = line;
	return res;
}


TArray<UueLine*> UueLine::GetLines(const CvLines& lines, float mx, float my)
{
	TArray<UueLine*> res;
	for (int i = 0; i < lines.size(); ++i)
	{
		res.Push(GetLine(lines[i]));
	}
	return res;
}

TArray<FVector2D> UueLine::GetLine_FV2Array(TArray<FVector2D> line, float mx, float my)
{
	TArray<FVector2D> res;
	res.SetNum(line.Num());
	for (int32 i = 0; i < line.Num(); ++i)
	{
		res[i] = line[i] + FVector2D(mx, my);
	}
	return res;
}

