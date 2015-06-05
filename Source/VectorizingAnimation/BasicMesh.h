#pragma once
#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <vector>
#include "LineDef.h"

struct BasicTraits : public OpenMesh::DefaultTraits
{
	typedef OpenMesh::Vec3d Point;
	FaceTraits
	{
	public:
		// for mapping id
		int32 mapid;
		// colorid
		int32 cid;
		// region id
		int32 rid;
		// region id
		int32 rid2;
		// hascolor
		char mark;
		// Color
		FVector c2;
		// for compute use
		FVector c3;

		FaceT() : cid(-1), rid(-1), mark(0) { }
	};
	EdgeTraits
	{
	public:
		// check is constraint
		char constraint;
		EdgeT(): constraint(0) { }
	};
	VertexTraits
	{
	public:
		// for constraint line index
		int32 lineidx;
		// for constraint line id
		int32 lineid;
		// for check is constraint point
		char constraint;
		// for check end point
		bool end;
		// for use
		bool use;
		// for interpolation color
		FVector c;
		// for sample color
		FVector c2;
		// for compute use
		FVector c3;
		// for mapping id
		int32 mapid;
		VertexT(): end(false), constraint(0), mapid(-1), lineid(-1) { }
	};
};

typedef OpenMesh::TriMesh_ArrayKernelT<BasicTraits> BasicMesh;

typedef std::vector<BasicMesh::FHandle> FHandles;
typedef std::vector<FHandles> FHandles2d;
typedef std::vector<BasicMesh::EHandle> EHandles;
typedef std::vector<EHandles> EHandles2d;
typedef std::vector<BasicMesh::VHandle> VHandles;
typedef std::vector<VHandles> VHandles2d;
typedef std::vector<BasicMesh::HHandle> HHandles;
typedef std::vector<HHandles> HHandles2d;
typedef std::vector<BasicMesh::HHandle*> HHandlePs;
typedef std::vector<BasicMesh::VHandle*> VHandlePs;
typedef std::vector<BasicMesh::FHandle*> FHandlePs;
