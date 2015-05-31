#pragma once
#include "BasicMesh.h"
#include "FlatVec3MeshLinear.h"
#include "TriangulationBase.h"
#include "LineV2Duplex.h"

class UTriangulationCgal_Sideline;
class PicMesh : public BasicMesh
{
public:

	BasicMesh::VertexIter vertices_begin()
	{
		return VertexIter(*this, VertexHandle(0));
	}

	//-----------------------------------------------------------------------------
	BasicMesh::VertexIter vertices_end()
	{
		return VertexIter(*this, VertexHandle(int(n_vertices())));
	}

	PicMesh(void);
	~PicMesh(void);
	// get middle point of face
	BasicMesh::Point MidPoint(FHandle fh);

	// ***********
	void ReadFromSideline(UTriangulationCgal_Sideline* data);
	// ***********
	// draw black color for no color
	void MakeColor1();
	// don't draw for no color
	void MakeColor2();

	// ***********
	// add region line by triangle information
	// lmax is max distance of connection line
	void MakeRegionLine(UcvMat* img, float lmax);
	// mapping next mesh
	void MappingMesh(PicMesh& pm, float x, float y);
	
	// ***********
	// Connect one ring edge, return true if success
	bool ConnectOneRingConstraint1(UcvMat* img, VHandle vh, VHandle lastvh, FVector2D& out, Point dir, float lmax);
	// Connect one ring edge, return true if success
	bool ConnectOneRingConstraint2(UcvMat* img, VHandle vh, VHandle lastvh, FVector2D& out, Point dir, float lmax);
	// check vertex connection
	bool isConnection(VHandle vh1, VHandle vh2);

	// ***********
	// set w h
	void SetSize(int w, int h);
	// draw color for color model

	// ***********
	void MakeColor6(UcvMat* img);
	// ***********
	void FillConstraintRegion();
	// ***********
	void MarkColoredFace();
	// ***********
	void GetConstraintFaceColor(UcvMat* img);
	
	// use picture build color model
	void BuildColorModels(UcvMat* img);
	// use mesh color build color model
	void BuildColorModels();
	// get lighter color
	bool GetLighterColor(UcvMat* img, VHandle vh1, VHandle vh2, FVector& c);
	// internel function
	void blurC2();
	// light blur
	void lightblurC2();

	// blurfacec2
	void blurFaceC2();
	
	int GetLeaveSize(int idx);

	// ***********
	void SetRegionColor(UcvMat* img);

	void MakeColorX9(int id);


	void ComputeRegion();

	int GetRegionId(float x, float y);
	// save w h
	int m_w, m_h;

	floats		m_RegionAreas;
	TArray<ULineV2*> m_Lines;
	VHandles2d	m_LinesVH;
	HHandles2d	m_LinesHH;
	FHandles2d	m_Regions;
	TArray<int32>		m_DifferentRegionIDs;
	TArray<int32>		m_RegionSize;
	// for next region
	TArray<int32>		m_MapingRegionIDs;
	TArray<int32>		m_MapingCount;
	TArray<FVector>		m_RegionColor;
	TArray<UIntsDuplex*>		m_i2s;
	TArray<FColorTriData>	m_Trangles;
	TArray<UFlatVec3MeshLinear*> m_ColorConstraint;
};

