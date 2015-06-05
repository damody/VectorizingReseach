// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineV3.h"

void ULineV3::Move(FVector vec)
{
	for (int32 i = 0; i < pts.Num(); ++i)
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

ULineV3* ULineV3::GetLine_Array(TArray<FVector> line)
{
	ULineV3* res = NewObject<ULineV3>();
	res->pts = line;
	return res;
}

TArray<FVector> ULineV3::GetLine_FV3Array(TArray<FVector> line, float mx, float my, float mz)
{
	TArray<FVector> res;
	res.SetNum(line.Num());
	for (int32 i = 0; i < line.Num(); ++i)
	{
		res[i] = line[i] + FVector(mx, my, mz);
	}
	return res;
}

