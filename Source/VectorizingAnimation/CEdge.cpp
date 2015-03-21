
#include "VectorizingAnimation.h"
#include "CEdge.h"

TArray<FVector2D> CEdge::GetLine(float mx, float my) const
{
	TArray<FVector2D> res;

	for (CvLine::const_iterator it = pnts.begin(); it != pnts.end(); ++it)
	{
		res.Add(FVector2D(it->x + mx, it->y + my));
	}
	return res;
}
