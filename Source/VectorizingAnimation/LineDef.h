#pragma once
#ifdef USE_CGAL
#pragma warning( push, 0 )
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#pragma warning( pop )
#endif // USE_CGAL
#include <memory.h>
#include <vector>
#include <opencv2/core/core.hpp>
#include <deque>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#ifdef USE_CGAL
typedef CGAL::Exact_predicates_inexact_constructions_kernel CgalInexactKernel;
typedef CgalInexactKernel::Point_2 CgalPoint;
typedef std::vector<CgalPoint> CgalLine;
typedef std::vector<CgalLine> CgalLines;
#endif // USE_CGAL

typedef std::vector<cv::Point> CvLine;
typedef std::vector<CvLine> CvLines;
