// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "VAFunction.h"
#include "ueMat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
// for FDesktopPlatformModule
#include "../DesktopPlatform/Public/DesktopPlatformModule.h"

bool UVAFunction::OpenFile(FString& path)
{
	TArray<FString> SelectedFiles;
	const FString Filter(TEXT("All files (*.*)|*.*"));
	bool bFileSelected = FDesktopPlatformModule::Get()->OpenFileDialog(
		nullptr,
		FString("Choose a file"),
		FString(""), TEXT(""), Filter, EFileDialogFlags::None, SelectedFiles
		);
	if (SelectedFiles.Num() > 0)
	{
		path = SelectedFiles[0];
		return true;
	}
	else
	{
		return false;
	}
}

void UVAFunction::Nop()
{
}

FVector2D UVAFunction::GetMoveVectorBySURF(UueMat* prevgray, UueMat* gray)
{
	prevgray->ConvertMat(EueMatEnum::UC_Gray);
	gray->ConvertMat(EueMatEnum::UC_Gray);
	cv::Mat a = prevgray->pic;
	cv::Mat b = gray->pic;
	int minHessian = 400;
	cv::SurfFeatureDetector detector(minHessian);
	std::vector<cv::KeyPoint> keypoints_1, keypoints_2;
	detector.detect(a, keypoints_1);
	detector.detect(b, keypoints_2);
	cv::SurfDescriptorExtractor extractor;
	cv::Mat descriptors_1, descriptors_2;
	extractor.compute(a, keypoints_1, descriptors_1);
	extractor.compute(b, keypoints_2, descriptors_2);
	cv::FlannBasedMatcher matcher;
	std::vector<cv::DMatch> matches;
	if (descriptors_2.cols == 0 || descriptors_1.cols == 0)
	{
		return FVector2D(0, 0);
	}
	matcher.match(descriptors_1, descriptors_2, matches);
	double max_dist = 0;
	double min_dist = 100;
	//-- Quick calculation of max and min distances between keypoints
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist)
		{
			min_dist = dist;
		}
		if (dist > max_dist)
		{
			max_dist = dist;
		}
	}
	std::vector<cv::DMatch> good_matches;
	for (int i = 0; i < descriptors_1.rows; i++)
	{
		if (matches[i].distance <= std::max(min_dist * 2, 0.02))
		{
			good_matches.push_back(matches[i]);
		}
	}
	cv::Vec2f mv(0, 0);
	for (int i = 0; i < good_matches.size(); i++)
	{
		cv::Point2f v = keypoints_2[good_matches[i].trainIdx].pt - keypoints_1[good_matches[i].queryIdx].pt;
		mv[0] += v.x;
		mv[1] += v.y;
	}
	mv /= (float)good_matches.size();
	mv[0] *= 10;
	mv[0] = ((int)mv[0]) * 0.1;
	mv[1] *= 10;
	mv[1] = ((int)mv[1]) * 0.1;
	if (mv[0] > 1000 || mv[0] < -1000)
	{
		mv[0] = 0;
	}
	if (mv[1] > 1000 || mv[1] < -1000)
	{
		mv[1] = 0;
	}
	return FVector2D(mv[0], mv[1]);
}


