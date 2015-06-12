// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "IPicMesh.h"

UIPicMesh* UIPicMesh::CreateIPicMeshInstance()
{
	return NewObject<UIPicMesh>();
}

void UIPicMesh::ReadFromSideline(UTriangulationCgal_Sideline* data)
{
	mesh.ReadFromSideline(data);
}

void UIPicMesh::SetSize(int32 w, int32 h)
{
	mesh.SetSize(w, h);
}

void UIPicMesh::MakeColor1()
{
	mesh.MakeColor1();
}

void UIPicMesh::SetRegionColor(UcvMat* img)
{
	mesh.SetRegionColor(img);
}

void UIPicMesh::ComputeRegion()
{
	mesh.ComputeRegion();
}

TArray<ULineV2*> UIPicMesh::GetConstructLines()
{
	return mesh.m_Lines;
}
