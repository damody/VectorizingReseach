// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "FlatVec3MeshLinear.h"
//#include "RBFMesh.h"

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
		for (int i = 0; i < Colors.Num(); i++)
		{
			sum += Colors[i];
		}
		ColorAverage = sum / Colors.Num();
	}
	else
	{
// 		UTriangulationCgal_SeedPoint triangulation;
// 		for (int i = 0; i < m_Colors.size(); ++i)
// 		{
// 			//if(canadd[cluster.at<int>(i, 0)])
// 			{
// 				Vector2& p = m_Pos[i];
// 				triangulation.AddPointIdx(p[0], p[1], i);
// 			}
// 		}
// 		m_rbfmesh.clear();
// 		m_rbfmesh.ReadFromSeedpoint(&triangulation, m_Colors);
	}
}

FVector UFlatVec3MeshLinear::GetColor(float x, float y)
{

	return FVector();
}
