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
	CEdge(int _index) {index = _index; }
	~CEdge(void) {}
	TArray<FVector2D> GetLine() const;
	// Domains assigned during link();
	int index;    // Start from 0
	int pointNum;
	cv::Point start, end;
	CvLine pnts;
};
typedef std::vector<CEdge> CEdges;


