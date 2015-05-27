// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ImageProcess.h"
#include "ueMat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

UueMat* UImageProcess::WhiteBalance(UueMat* umat)
{
	UueMat* ures = NewObject<UueMat>();
	cv::Mat input, oriImg = umat->pic;
	cvtColor(oriImg, input, CV_BGR2YCrCb);
	// 先用直方圖得到門檻
	cv::Mat res = oriImg.clone();
	int histRGB[768] = { 0 };
	int Amount = 0;
	double AvgB = 0, AvgG = 0, AvgR = 0;
	const double Ratio = 0.5;
	for (int a = 0; a < res.rows; ++a)
	{
		for (int b = 0; b < res.cols; ++b)
		{
			cv::Vec3b cid1 = res.at<cv::Vec3b>(a, b);
			int sum = (int)cid1[0] + (int)cid1[1] + (int)cid1[2];
			histRGB[sum]++;
		}
	}
	int Sum = 0;
	int Threshold = 0;
	for (int Y = 767; Y >= 0; Y--)
	{
		Sum += histRGB[Y];
		if (Sum > res.cols * res.rows * Ratio / 100)
		{
			Threshold = Y;
			break;
		}
	}
	// 得到大於門檻的平均色
	for (int a = 0; a < res.rows; ++a)
	{
		for (int b = 0; b < res.cols; ++b)
		{
			cv::Vec3b cid1 = res.at<cv::Vec3b>(a, b);
			int sum = (int)cid1[0] + (int)cid1[1] + (int)cid1[2];
			if (sum > Threshold)
			{
				AvgB += cid1[0];
				AvgG += cid1[1];
				AvgR += cid1[2];
				Amount++;
			}
		}
	}
	AvgB /= Amount;
	AvgG /= Amount;
	AvgR /= Amount;
	const int MaxValue = 255;
	for (int a = 0; a < res.rows; ++a)
	{
		for (int b = 0; b < res.cols; ++b)
		{
			cv::Vec3b& cid1 = res.at<cv::Vec3b>(a, b);
			int Blue = cid1[0] * MaxValue / AvgB;
			int Green = cid1[1] * MaxValue / AvgG;
			int Red = cid1[2] * MaxValue / AvgR;
			if (Red > 255)
			{
				Red = 255;
			}
			else if (Red < 0)
			{
				Red = 0;
			}
			if (Green > 255)
			{
				Green = 255;
			}
			else if (Green < 0)
			{
				Green = 0;
			}
			if (Blue > 255)
			{
				Blue = 255;
			}
			else if (Blue < 0)
			{
				Blue = 0;
			}
			cid1[0] = Blue;
			cid1[1] = Green;
			cid1[2] = Red;
		}
	}
	ures->pic = res;
	return ures;
}

UueMat* UImageProcess::ImgSharpen(UueMat* umat)
{
	UueMat* ures = NewObject<UueMat>();
	cv::Mat oriImg = umat->pic;
	cv::Mat image = oriImg.clone(), res;
	cv::GaussianBlur(image, res, cv::Size(0, 0), 3);
	float p = 0.6;
	cv::addWeighted(image, 1 + p, res, -p, 0, res);
	ures->pic = res;
	return ures;
}

UueMat* UImageProcess::PowerEnhance(UueMat* umat, float power)
{
	UueMat* ures = NewObject<UueMat>();
	umat->ConvertMat(EueMatEnum::FC_BGR);
	cv::Mat oriImg = umat->pic;
	cv::Mat res = oriImg.clone();
	for (int j = 0; j < res.cols; ++j)
	{
		for (int i = 0; i < res.rows; ++i)
		{
			cv::Vec3f& intensity = res.at<cv::Vec3f>(i, j);
			intensity[0] = -pow((1 - intensity[0]), 3);
			intensity[1] = -pow((1 - intensity[1]), 3);
			intensity[2] = -pow((1 - intensity[2]), 3);
		}
	}
	ures->pic = res;
	return ures;
}

UueMat* UImageProcess::NormalizeUC(UueMat* umat, float vmin, float vmax)
{
	UueMat* ures = NewObject<UueMat>();
	EueMatEnum eum = umat->GetMatState();
	cv::Mat ori = umat->pic.clone();
	switch (eum)
	{
	case EueMatEnum::Error:
		break;
	case EueMatEnum::UC_Gray:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::UC_BGR:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::FC_Gray:
		ori.clone().convertTo(ori, CV_8UC1, 1.0 / 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::FC_BGR:
		ori.clone().convertTo(ori, CV_8UC1, 1.0 / 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	default:
		break;
	}
	ures->pic = ori;
	return ures;
}

UueMat* UImageProcess::NormalizeFC(UueMat* umat, float vmin, float vmax)
{
	UueMat* ures = NewObject<UueMat>();
	EueMatEnum eum = umat->GetMatState();
	cv::Mat ori = umat->pic.clone();
	switch (eum)
	{
	case EueMatEnum::Error:
		break;
	case EueMatEnum::UC_Gray:
		ori.clone().convertTo(ori, CV_32FC1, 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::UC_BGR:
		ori.clone().convertTo(ori, CV_32FC1, 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::FC_Gray:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EueMatEnum::FC_BGR:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	default:
		break;
	}
	ures->pic = ori;
	return ures;
}

UueMat* UImageProcess::BilateralFilter(UueMat* umat, int32 maskSize)
{
	UueMat* ures = NewObject<UueMat>();
	umat->ConvertMat(EueMatEnum::UC_BGR);
	cv::Mat ori = umat->pic.clone();
	for (int i = 1; i < 7; i++)
	{
		bilateralFilter(ori.clone(), ori, i, i * 2, i * 0.5);
	}
	ures->pic = ori;
	return ures;
}
