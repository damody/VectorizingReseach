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

bool UCurveExtraction::Set_ueMat(UueMat* umat)
{
	m_CurvePic = umat->Pic.clone();
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

UueMat* UCurveExtraction::CalSecDer(int32 maskSize, double linkEndBound, double linkStartBound)
{
	UueMat* ures = NewObject<UueMat>();
	return ures;
}
