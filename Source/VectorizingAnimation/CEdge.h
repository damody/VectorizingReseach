#pragma once
#include <opencv2/core/core.hpp>
#include "LineDef.h"
#include "Runtime/Core/Public/Math/Vector2D.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include <opencv2/core/core.hpp>
#include <vector>

class CEdge
{
public:
	CEdge(int32 _index) {index = _index; }
	~CEdge(void) {}
	TArray<FVector2D> GetLine(float mx, float my) const;
	// Domains assigned during link();
	int32 index;    // Start from 0
	int32 pointNum;
	cv::Point start, end;
	CvLine pnts;
};
typedef std::vector<CEdge> CEdges;


