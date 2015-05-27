// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "CurveExtraction.h"
#include "ueMat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


UCurveExtraction* UCurveExtraction::NewCurveExtraction()
{
	return NewObject<UCurveExtraction>();
}

bool UCurveExtraction::SetPicture(UueMat* umat)
{
	m_CurvePic = umat->pic.clone();
	switch (m_CurvePic.type())
	{
	case CV_8UC3:
		cvtColor(m_CurvePic, m_CurvePic, CV_BGR2GRAY);
		break;
	case CV_8UC1:
		break;
	case CV_32FC3:
		cvtColor(m_CurvePic, m_CurvePic, CV_BGR2GRAY);
		m_CurvePic.clone().convertTo(m_CurvePic, CV_8UC1, 255);
		break;
	case CV_32FC1:
		m_CurvePic.clone().convertTo(m_CurvePic, CV_8UC1, 255);
		break;
	}
	m_CmCurveEx = CmCurveEx(m_CurvePic);
	return true;
}

TArray<UueLine*> UCurveExtraction::CalSecDer(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UueMat* ures = NewObject<UueMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalSecDer(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(UueLine::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}

TArray<UueLine*> UCurveExtraction::CalSecDer2(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UueMat* ures = NewObject<UueMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalSecDer2(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(UueLine::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}

TArray<UueLine*> UCurveExtraction::CalFirDer(int32 maskSize, float linkEndBound, float linkStartBound)
{
	UueMat* ures = NewObject<UueMat>();
	if (m_CurvePic.cols == 0 || m_CurvePic.rows == 0)
	{
		return Curves;
	}
	ures->Copy(m_CmCurveEx.CalFirDer(maskSize, linkEndBound, linkStartBound));
	m_CmCurveEx.Link();
	const std::vector<CEdge>& edges = m_CmCurveEx.GetEdges();
	for (size_t i = 0; i < edges.size(); i++)
	{
		Curves.Push(UueLine::GetLine_Array(edges[i].GetLine(0.5f, 0.5f)));
	}
	m_CurveLink = ures;
	return Curves;
}
