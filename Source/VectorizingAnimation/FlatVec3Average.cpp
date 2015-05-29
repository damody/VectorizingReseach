// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "FlatVec3Average.h"


void UFlatVec3Average::BuildModel()
{
	NeedBuildModel = false;

	if (Colors.Num()>0)
	{
		FVector sum;
		for (int i=0;i<Colors.Num();i++)
		{
			sum += Colors[i];
		}
		ColorAverage = sum / Colors.Num();
	}
}

FVector UFlatVec3Average::GetColor()
{
	if (NeedBuildModel)
	{
		BuildModel();
	}
	return ColorAverage;
}
