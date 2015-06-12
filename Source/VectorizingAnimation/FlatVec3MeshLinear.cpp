// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "TriangulationCgal_SeedPoint.h"
#include "FlatVec3MeshLinear.h"
#include "RBFMesh.h"

void UFlatVec3MeshLinear::BuildModel()
{
	NeedBuildModel = false;
	if (Colors.Num() == 0)
	{
		return;
	}
	if (Colors.Num() < 10)
	{
		FVector sum;
		for (int32 i = 0; i < Colors.Num(); i++)
		{
			sum += Colors[i];
		}
		ColorAverage = sum / Colors.Num();
	}
	else
	{
		UTriangulationCgal_SeedPoint triangulation;
		for (int32 i = 0; i < Colors.Num(); ++i)
		{
			//if(canadd[cluster.at<int32>(i, 0)])
			{
				FVector2D& p = Pos[i];
				triangulation.AddPointIdx(p[0], p[1], i);
			}
		}
		mesh.clear();
		mesh.ReadFromSeedpoint(&triangulation, Colors);
	}
}

UFlatVec3MeshLinear* UFlatVec3MeshLinear::Clone()
{
	UFlatVec3MeshLinear* res = NewObject<UFlatVec3MeshLinear>();
	res->NeedBuildModel = this->NeedBuildModel;
	res->Pos = this->Pos;
	res->Colors = this->Colors;
	res->mesh = this->mesh;
	res->ColorAverage = this->ColorAverage;
	return res;
}

TArray<UFlatVec3MeshLinear*> UFlatVec3MeshLinear::CloneArray(const TArray<UFlatVec3MeshLinear*>& src)
{
	TArray<UFlatVec3MeshLinear*> res;
	res.Reset(src.Num());
	for (int32 i = 0; i < src.Num(); ++i)
	{
		res[i] = src[i]->Clone();
	}
	return res;
}

FVector UFlatVec3MeshLinear::GetColor2Point(float x1, float y1, float x2, float y2)
{
	if (NeedBuildModel)
	{
		BuildModel();
	}
	if (Colors.Num() < 10)
	{
		return ColorAverage;
	}
	return mesh.GetColor2Point(x1, y1, x2, y2);
}

FVector UFlatVec3MeshLinear::GetColor(float x, float y)
{
	if (NeedBuildModel)
	{
		BuildModel();
	}
	if (Colors.Num() < 10)
	{
		return ColorAverage;
	}
	return mesh.GetColor(x, y);
}
