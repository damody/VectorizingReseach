// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "FlatVec3Model.h"



void UFlatVec3Model::AddPoint(float x, float y, FVector color)
{
	Pos.Add(FVector2D(x, y));
	Colors.Add(color);
}

void UFlatVec3Model::AddPoint_V2D(FVector2D pos, FVector color)
{
	Pos.Add(pos);
	Colors.Add(color);
}
