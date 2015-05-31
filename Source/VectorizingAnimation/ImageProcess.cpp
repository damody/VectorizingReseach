// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ImageProcess.h"
#include "cvMat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

UcvMat* UImageProcess::WhiteBalance(UcvMat* umat)
{
	umat->ConvertMat(EcvMatEnum::UC_BGR);
	UcvMat* ures = NewObject<UcvMat>();
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

UcvMat* UImageProcess::ImgSharpen(UcvMat* umat)
{
	umat->ConvertMat(EcvMatEnum::UC_BGR);
	UcvMat* ures = NewObject<UcvMat>();
	cv::Mat oriImg = umat->pic;
	cv::Mat image = oriImg.clone(), res;
	cv::GaussianBlur(image, res, cv::Size(0, 0), 3);
	float p = 0.6;
	cv::addWeighted(image, 1 + p, res, -p, 0, res);
	ures->pic = res;
	return ures;
}

UcvMat* UImageProcess::PowerEnhance(UcvMat* umat, float power)
{
	UcvMat* ures = NewObject<UcvMat>();
	umat->ConvertMat(EcvMatEnum::FC_BGR);
	cv::Mat res = umat->pic.clone();
	for (int j = 0; j < res.cols; ++j)
	{
		for (int i = 0; i < res.rows; ++i)
		{
			cv::Vec3f& intensity = res.at<cv::Vec3f>(i, j);
			intensity[0] = -pow((1 - intensity[0]), power);
			intensity[1] = -pow((1 - intensity[1]), power);
			intensity[2] = -pow((1 - intensity[2]), power);
		}
	}
	normalize(res, res, 0, 1, cv::NORM_MINMAX);
	ures->pic = res;
	return ures;
}

UcvMat* UImageProcess::NormalizeUC(UcvMat* umat, float vmin, float vmax)
{
	UcvMat* ures = NewObject<UcvMat>();
	EcvMatEnum eum = umat->GetMatState();
	cv::Mat ori = umat->pic.clone();
	switch (eum)
	{
	case EcvMatEnum::Error:
		break;
	case EcvMatEnum::UC_Gray:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::UC_BGR:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::FC_Gray:
		ori.clone().convertTo(ori, CV_8UC1, 1.0 / 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::FC_BGR:
		ori.clone().convertTo(ori, CV_8UC1, 1.0 / 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	default:
		break;
	}
	ures->pic = ori;
	return ures;
}

UcvMat* UImageProcess::NormalizeFC(UcvMat* umat, float vmin, float vmax)
{
	UcvMat* ures = NewObject<UcvMat>();
	EcvMatEnum eum = umat->GetMatState();
	cv::Mat ori = umat->pic.clone();
	switch (eum)
	{
	case EcvMatEnum::Error:
		break;
	case EcvMatEnum::UC_Gray:
		ori.clone().convertTo(ori, CV_32FC1, 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::UC_BGR:
		ori.clone().convertTo(ori, CV_32FC1, 255);
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::FC_Gray:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	case EcvMatEnum::FC_BGR:
		normalize(ori, ori, vmin, vmax, cv::NORM_MINMAX);
		break;
	default:
		break;
	}
	ures->pic = ori;
	return ures;
}

UcvMat* UImageProcess::BilateralFilter(UcvMat* umat, int32 maskSize)
{
	umat->ConvertMat(EcvMatEnum::UC_BGR);
	UcvMat* ures = NewObject<UcvMat>();
	cv::Mat ori = umat->pic.clone();
	for (int i = 1; i < 7; i++)
	{
		bilateralFilter(ori.clone(), ori, i, i * 2, i * 0.5);
	}
	ures->pic = ori;
	return ures;
}

static void S6FloodFill(cv::Mat& image, cv::Mat& mask01, int& cc, int x, int y)
{
	if (image.at<cv::Vec3b>(y, x) == cv::Vec3b(255, 255, 255)
		|| mask01.at<uchar>(y + 1, x + 1) > 0)
	{
		return;
	}
	int b = cc % 256;
	int g = (cc / 256) % 256;
	int r = cc / 256 / 256;
	cv::Point seed(x, y);
	cv::Rect ccomp;
	cv::Scalar newVal(b, g, r);
	int area;
	int lo = 0;
	int up = 0;
	threshold(mask01, mask01, 1, 128, CV_THRESH_BINARY);
	int flags = 4 + (255 << 8) + CV_FLOODFILL_FIXED_RANGE;
	area = floodFill(image, mask01, seed, newVal, &ccomp, cv::Scalar(lo, lo, lo),
		cv::Scalar(up, up, up), flags);
	cc++;
}

TArray<UFlatVec3MeshLinear*> UImageProcess::BuildColorModelV8(UcvMat* image, UcvMat* oimg, UcvMat* mask2)
{
	image->UpdateTextureToCvMat();
	TArray<UFlatVec3MeshLinear*> ccms;
	cv::Mat mask;
	//cv::resize(oimg.clone(), oimg, oimg.size() * 2, 0, 0, cv::INTER_CUBIC);
	mask.create(image->pic.rows + 2, image->pic.cols + 2, CV_8UC1);
	mask = cv::Scalar::all(0);
	int cc = 1;
	for (int i = 0; i < image->pic.rows; i++)
	{
		for (int j = 0; j < image->pic.cols - 1; j++)
		{
			int lastcc = cc;
			S6FloodFill(image->pic, mask, cc, j, i);
			if (lastcc != cc)
			{
				UFlatVec3MeshLinear* ccm = NewObject<UFlatVec3MeshLinear>();
				for (int ii = 0; ii < image->pic.rows; ii++)
				{
					for (int jj = 0; jj < image->pic.cols; jj++)
					{
						if (mask.at<uchar>(ii + 1, jj + 1) > 130 && mask2->pic.at<uchar>(ii, jj) == 0)
						{
							ccm->AddPoint(jj, ii, Vec3bToVector(oimg->pic.at<cv::Vec3b>(ii, jj)));
						}
					}
				}
				ccms.Add(ccm);
			}
		}
	}
	return ccms;
}

UcvMat* UImageProcess::DrawWhileLine(UcvMat* umat, const TArray<ULineV2*> draw)
{
	UcvMat* ures = NewObject<UcvMat>();
	umat->ConvertMat(EcvMatEnum::UC_BGR);
	cv::Mat ori = umat->pic.clone();
	for (int i = 0; i < draw.Num(); ++i)
	{
		ULineV2& nowl = *draw[i];
		for (int j = 0; j < nowl.Num() - 1; ++j)
		{
			cv::Point p1(nowl[j].X, nowl[j].Y);
			cv::Point p2(nowl[j + 1].X, nowl[j + 1].Y);
			cv::line(ori, p1, p2, cv::Scalar(255, 255, 255));
		}
	}
	ures->pic = ori;
	return ures;
}

struct WeightData
{
	WeightData(FVector2D p, int w) : pos(p), weight(w)    {}
	FVector2D pos;
	int  weight;
	bool operator<(const WeightData& wd)
	{
		return weight < wd.weight;
	}
};
typedef std::vector<WeightData> Weights;

UcvMat* UImageProcess::FixSpaceLineX(UcvMat* umat, UcvMat* oriImg, float initdis)
{
	UcvMat* ures = NewObject<UcvMat>();
	umat->ConvertMat(EcvMatEnum::UC_BGR);
	cv::Mat res = umat->pic.clone();

	Weights wm_init_cross;
	wm_init_cross.push_back(WeightData(FVector2D(0, -1), 1));
	wm_init_cross.push_back(WeightData(FVector2D(0, +1), 1));
	wm_init_cross.push_back(WeightData(FVector2D(-1, 0), 1));
	wm_init_cross.push_back(WeightData(FVector2D(+1, 0), 1));
	for (int a = 0; a < res.rows; ++a)
	{
		for (int b = 0; b < res.cols; ++b)
		{
			cv::Vec3b& cid1 = res.at<cv::Vec3b>(a, b);
			if (cid1[0] == 255 && cid1[1] == 255 && cid1[2] == 255)
			{
				cv::Vec3b rescolor(255, 255, 255);
				cv::Vec3b oricolor = oriImg->pic.at<cv::Vec3b>(a, b);
				double dis = initdis;
				FVector oriv(oricolor[0], oricolor[1], oricolor[1]);
				Weights wm = wm_init_cross;
				for (int i = 1; i < wm.size(); i += 2)
				{
					int y = a + wm[i].pos.Y;
					int x = b + wm[i].pos.X;
					if (y >= 0 && x >= 0 && y < umat->pic.rows && x < umat->pic.cols)
					{
						cv::Vec3b nn = umat->pic.at<cv::Vec3b>(y, x);
						if (!(nn[0] == 255 && nn[1] == 255 && nn[2] == 255))
						{
							cv::Vec3b oricolor2 = oriImg->pic.at<cv::Vec3b>(a + wm[i].pos.Y, b + wm[i].pos.X);
							FVector oriv2(oricolor2[0], oricolor2[1], oricolor2[1]);
							if (FVector::DistSquared(oriv2, oriv) < dis)
							{
								dis = FVector::DistSquared(oriv2, oriv);
								cid1 = nn;
							}
						}
					}
				}
			}
		}
	}
	ures->pic = res;
	return ures;
}
