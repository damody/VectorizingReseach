// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "CurveExtraction.h"
#include "cvMat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


UCurveExtraction* UCurveExtraction::CreateCurveExtractionInstance()
{
	return NewObject<UCurveExtraction>();
}

bool UCurveExtraction::SetPicture(UcvMat* umat)
{
	m_CurvePic = umat->Clone()->ConvertMat(EcvMatEnum::FC_Gray)->pic.clone();
	m_CmCurveEx = CmCurveEx(m_CurvePic);
	return true;
}

TArray<ULineV2*> UCurveExtraction::CalSecDer(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UcvMat* ures = NewObject<UcvMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalSecDer(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(ULineV2::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}

TArray<ULineV2*> UCurveExtraction::CalSecDer2(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UcvMat* ures = NewObject<UcvMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalSecDer2(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(ULineV2::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}

TArray<ULineV2*> UCurveExtraction::CalFirDer(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UcvMat* ures = NewObject<UcvMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalFirDer(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(ULineV2::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}
