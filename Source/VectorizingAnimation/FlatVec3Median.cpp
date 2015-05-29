// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "FlatVec3Median.h"
#include "SortFunction.h"


void UFlatVec3Median::BuildModel()
{
	NeedBuildModel = false;
	if (Colors.Num() > 0)
	{
		Colors.Sort(LightCompareVector3);
		ColorMedian = Colors[Colors.Num() / 2];
	}
}

FVector UFlatVec3Median::GetColor()
{
	if (NeedBuildModel)
	{
		BuildModel();
	}
	return ColorMedian;
}
