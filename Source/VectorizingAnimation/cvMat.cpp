// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "GeneralDef.h"
#include "cvMat.h"
#include "RHI.h"
#include "Engine/Texture2D.h"


void UcvMat::Copy(cv::Mat input)
{
    pic = input.clone();
}

UcvMat* UcvMat::UpdateCvMatToTexture()
{
    if(pic.cols == 0)
    {
        pic.create(32, 32, CV_8UC3);
        for(int32 y = 0; y < pic.rows; y++)
        {
            for(int32 x = 0; x < pic.cols; x++)
            {
                cv::Vec3b& t = pic.at<cv::Vec3b>(y, x);
                t[0] = rand() % 256;
                t[1] = rand() % 256;
                t[2] = rand() % 256;
            }
        }
    }
	const int32 SizeX = pic.cols;
	const int32 SizeY = pic.rows;
    Texture = UTexture2D::CreateTransient(SizeX, SizeY, PF_B8G8R8A8);
	//Texture->SRGB = 0;
    FColor* MipData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
    if(pic.type() == CV_8UC3)
    {
        for(int32 y = 0; y < pic.rows; y++)
        {
            for(int32 x = 0; x < pic.cols; x++)
            {
                int32 curPixelIndex = (y * pic.cols) + x;
                FColor& c = MipData[curPixelIndex];
                cv::Vec3b& t = pic.at<cv::Vec3b>(y, x);
                c.B = t[0];
                c.G = t[1];
                c.R = t[2];
            }
        }
    }
    else if(pic.type() == CV_8UC1)
    {
        for(int32 y = 0; y < pic.rows; y++)
        {
            for(int32 x = 0; x < pic.cols; x++)
            {
                int32 curPixelIndex = (y * pic.cols) + x;
                FColor& c = MipData[curPixelIndex];
                uchar& t = pic.at<uchar>(y, x);
                c.B = t;
                c.G = t;
                c.R = t;
            }
        }
    }
    // Unlock the texture
    Texture->PlatformData->Mips[0].BulkData.Unlock();
    Texture->UpdateResource();
    return this;
}

void UcvMat::UpdateTextureToCvMat()
{
    Texture->CompressionSettings = TC_EditorIcon;
    Texture->UpdateResourceW();
    FColor* FormatedImageData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

    pic.create(Texture->PlatformData->Mips[0].SizeY, Texture->PlatformData->Mips[0].SizeX, CV_8UC3);
    pic = cv::Scalar(0);
    for(int32 y = 0; y < Texture->PlatformData->Mips[0].SizeY; y++)
    {
        for(int32 x = 0; x < Texture->PlatformData->Mips[0].SizeX; x++)
        {
            int32 curPixelIndex = (y * Texture->PlatformData->Mips[0].SizeX) + x;
            FColor c = FormatedImageData[curPixelIndex];
            cv::Vec3b& t = pic.at<cv::Vec3b>(y, x);
            t[0] = c.B;
            t[1] = c.G;
            t[2] = c.R;
        }
    }
    Texture->PlatformData->Mips[0].BulkData.Unlock();
}

UMaterialInterface* UcvMat::MakeMaterial()
{
    UMaterialInstanceDynamic* RV_MatInst = UMaterialInstanceDynamic::Create(MasterMaterialRef, this);
    RV_MatInst->SetTextureParameterValue(FName("T2DParam"), Texture);
    return MasterMaterialRef;
}


EcvMatEnum UcvMat::GetMatState()
{
    switch(pic.type())
    {
    case CV_8UC3:
        return EcvMatEnum::UC_BGR;
    case CV_8UC1:
        return EcvMatEnum::UC_Gray;
    case CV_32FC3:
        return EcvMatEnum::FC_BGR;
    case CV_32FC1:
        return EcvMatEnum::FC_Gray;
    }
    return EcvMatEnum::Error;
}

void UcvMat::ConvertMat(EcvMatEnum state)
{
    switch(state)
    {
    case EcvMatEnum::UC_BGR:
    {
        switch(pic.type())
        {
        case CV_8UC3:
            break;
        case CV_8UC1:
            cvtColor(pic.clone(), pic, CV_GRAY2BGR);
            break;
        case CV_32FC3:
            pic.clone().convertTo(pic, CV_8UC1, 255);
            break;
        case CV_32FC1:
            pic.clone().convertTo(pic, CV_8UC1, 255);
            cvtColor(pic.clone(), pic, CV_GRAY2BGR);
            break;
        }
        return;
    }
    case EcvMatEnum::UC_Gray:
    {
        switch(pic.type())
        {
        case CV_8UC3:
            cvtColor(pic.clone(), pic, CV_BGR2GRAY);
            break;
        case CV_8UC1:
            break;
        case CV_32FC3:
            pic.clone().convertTo(pic, CV_8UC1, 255);
            cvtColor(pic.clone(), pic, CV_BGR2GRAY);
            break;
        case CV_32FC1:
            pic.clone().convertTo(pic, CV_8UC1, 255);
            break;
        }
        return;
    }
    case EcvMatEnum::FC_BGR:
    {
        switch(pic.type())
        {
        case CV_8UC3:
            pic.clone().convertTo(pic, CV_32FC1, 1.0 / 255);
            break;
        case CV_8UC1:
            pic.clone().convertTo(pic, CV_32FC1, 1.0 / 255);
            cvtColor(pic.clone(), pic, CV_GRAY2BGR);
            break;
        case CV_32FC3:
            break;
        case CV_32FC1:
            cvtColor(pic.clone(), pic, CV_GRAY2BGR);
            break;
        }
        return;
    }
    case EcvMatEnum::FC_Gray:
    {
        switch(pic.type())
        {
        case CV_8UC3:
            pic.clone().convertTo(pic, CV_32FC1, 1.0 / 255);
            cvtColor(pic.clone(), pic, CV_BGR2GRAY);
            break;
        case CV_8UC1:
            pic.clone().convertTo(pic, CV_32FC1, 1.0 / 255);
            break;
        case CV_32FC3:
            cvtColor(pic.clone(), pic, CV_BGR2GRAY);
            break;
        case CV_32FC1:
            break;
        }
        return;
        break;
    }
    }
}

UcvMat* UcvMat::Clone()
{
	UcvMat* res = NewObject<UcvMat>();
	res->pic = this->pic.clone();
	return res;
}

TArray<UcvMat*> UcvMat::MakeStaticBackGroundByMove(const TArray<UcvMat*>& m_Video, TArray<FVector2D>& m_Moves, UcvMat* background, UcvMat* foreground)
{
	TArray<UcvMat*> nullres;
	if (m_Video.Num() != m_Moves.Num())
	{
		//printf("m_Video.size() %d  m_Moves.size() %d\n", m_Video.size(), m_Moves.size());
		return nullres;
	}
	else
	{
 		int maxX = 0, maxY = 0, minX = 0, minY = 0;
 		for (int i = 0; i < m_Moves.Num(); ++i)
		{
			if (m_Moves[i][0] > maxX)
			{
				maxX = m_Moves[i][0] + 1;
			}
			if (m_Moves[i][1] > maxY)
			{
				maxY = m_Moves[i][1] + 1;
			}
			if (m_Moves[i][0] < minX)
			{
				minX = m_Moves[i][0] - 1;
			}
			if (m_Moves[i][1] < minY)
			{
				minY = m_Moves[i][1] - 1;
			}
		}
		int32 ow = m_Video.Last()->pic.cols;
		int32 oh = m_Video.Last()->pic.rows;
		const int32 finalW = ow + maxX - minX + 1;
		const int32 finalH = oh + maxY - minY + 1;
 		//printf("finalW %d finalH %d\n", finalW, finalH);
 		cv::Mat bgimg, bgcount;
 		TArray<UcvMat*> timgs;
 		bgimg.create(finalH, finalW, CV_32FC3);
 		bgcount.create(finalH, finalW, CV_32FC1);
 		bgimg = cv::Scalar(0);
 		for (int a = 0; a < m_Video.Num(); ++a)
 		{
 			bgcount = cv::Scalar(0);
 			cv::Mat timg;
 			timg.create(finalH, finalW, CV_8UC3);
 			timg = cv::Scalar(0);
 			cv::Mat img = *m_Video[a];
 			FVector2D mv = m_Moves[a];
 			float x = mv[0] - minX;
 			float y = mv[1] - minY;
			int32 fx = floor(x);
			int32 fy = floor(y);
			float lw = 1 - (x - floor(x));
			float rw = 1 - lw;
			float tw = 1 - (y - floor(y));
			float bw = 1 - tw;
			float lt = lw * tw, lb = lw * bw, rt = rw * tw, rb = rw * bw;
			//printf("x %.2f y %.2f lw %.2f rw %.2f tw %.2f bw %.2f\n", x, y, lw, rw, tw, bw);
			for (int32 i = 0; i < img.rows; ++i)
			{
				for (int32 j = 0; j < img.cols; ++j)
				{
					cv::Vec3b& tv00 = timg.at<cv::Vec3b>(fy + i, fx + j);
					cv::Vec3b& tv10 = timg.at<cv::Vec3b>(fy + i + 1, fx + j);
					cv::Vec3b& tv01 = timg.at<cv::Vec3b>(fy + i, fx + j + 1);
					cv::Vec3b& tv11 = timg.at<cv::Vec3b>(fy + i + 1, fx + j + 1);
					float& c00 = bgcount.at<float>(fy + i, fx + j);
					float& c10 = bgcount.at<float>(fy + i + 1, fx + j);
					float& c01 = bgcount.at<float>(fy + i, fx + j + 1);
					float& c11 = bgcount.at<float>(fy + i + 1, fx + j + 1);
					c00 += lt;
					c10 += lb;
					c01 += rt;
					c11 += rb;
					tv00[0] += img.at<cv::Vec3b>(i, j)[0] * lt;
					tv00[1] += img.at<cv::Vec3b>(i, j)[1] * lt;
					tv00[2] += img.at<cv::Vec3b>(i, j)[2] * lt;
					tv10[0] += img.at<cv::Vec3b>(i, j)[0] * lb;
					tv10[1] += img.at<cv::Vec3b>(i, j)[1] * lb;
					tv10[2] += img.at<cv::Vec3b>(i, j)[2] * lb;
					tv01[0] += img.at<cv::Vec3b>(i, j)[0] * rt;
					tv01[1] += img.at<cv::Vec3b>(i, j)[1] * rt;
					tv01[2] += img.at<cv::Vec3b>(i, j)[2] * rt;
					tv11[0] += img.at<cv::Vec3b>(i, j)[0] * rb;
					tv11[1] += img.at<cv::Vec3b>(i, j)[1] * rb;
					tv11[2] += img.at<cv::Vec3b>(i, j)[2] * rb;
				}
			}
			for (int i = 0; i < img.rows + 1; ++i)
			{
				for (int j = 0; j < img.cols + 1; ++j)
				{
					float& c00 = bgcount.at<float>(fy + i, fx + j);
					if (abs(c00 - 1) > 0.01)
					{
						cv::Vec3b& tv00 = timg.at<cv::Vec3b>(fy + i, fx + j);
						tv00 /= c00;
					}
				}
			}
			timgs.Add(NewObject<UcvMat>(new UcvMat(timg)));
 		}
		cv::Mat meanimg;
		meanimg = timgs[0]->pic.clone();
		ints useids;
		ImgFillBlack(timgs[0]->pic, meanimg);
		ImgFillBlack(timgs.Last()->pic, meanimg);
		//cv::imwrite("meanimg.png", meanimg);
		for (int k = 1; k < timgs.Num() - 1; ++k)
		{
			float mindis = ColorDistance(meanimg, timgs[k + 1]->pic);
			int idx = k + 1;
			for (int j = k + 2; j <= k + 10 && j < timgs.Num(); ++j)
			{
				float dis = ColorDistance(meanimg, timgs[k + 1]->pic);
				if (dis < mindis)
				{
					mindis = dis;
					idx = j;
				}
			}
			k = idx;
			ImgFillBlack(timgs[idx]->pic, meanimg);
			useids.Add(idx);
		}
		cv::Mat errimg;
		errimg.create(finalH, finalW, CV_32FC1);
		cv::Vec3b black(0, 0, 0);
		for (int k = 0; k < timgs.Num() - 1; ++k)
		{
			for (int i = 0; i < errimg.rows; ++i)
			{
				for (int j = 0; j < errimg.cols; ++j)
				{
					cv::Vec3b tc1 = timgs[k]->pic.at<cv::Vec3b>(i, j);
					cv::Vec3b tc2 = timgs[k + 1]->pic.at<cv::Vec3b>(i, j);
					if (tc1 != black && tc2 != black)
					{
						float aa = abs(tc2[0] - tc1[0]);
						float bb = abs(tc2[1] - tc1[1]);
						float cc = abs(tc2[2] - tc1[2]);
						errimg.at<float>(i, j) += aa * aa;
						errimg.at<float>(i, j) += bb * bb;
						errimg.at<float>(i, j) += cc * cc;
					}
				}
			}
		}
		normalize(errimg, errimg, 0, 1, cv::NORM_MINMAX);
		for (int i = 0; i < errimg.rows; ++i)
		{
			for (int j = 0; j < errimg.cols; ++j)
			{
				if (errimg.at<float>(i, j) > 0.2)
				{
					errimg.at<float>(i, j) = 1;
				}
				else
				{
					errimg.at<float>(i, j) = 0;
				}
			}
		}
		cv::Mat forDilation;
		errimg.convertTo(forDilation, CV_8UC1, 255);
		foreground->pic = forDilation;
		background->pic = meanimg;
		//printf("minX %d maxX %d minY %d maxY %d \n", minX, maxX, minY, maxY);
		return timgs;
 	}
	return nullres;
}

float UcvMat::ColorDistance(cv::Mat& a, cv::Mat& b)
{
	if (a.cols == b.cols && a.rows == b.rows)
	{
		cv::Vec3b black(0, 0, 0);
		int count = 0;
		cv::Vec3d diff;
		for (int i = 0; i < a.rows; ++i)
		{
			for (int j = 0; j < b.cols; ++j)
			{
				cv::Vec3b& ca = a.at<cv::Vec3b>(i, j);
				cv::Vec3b& cb = b.at<cv::Vec3b>(i, j);
				if (ca != black && cb != black)
				{
					count++;
					diff[0] += abs(ca[0] - cb[0]);
					diff[1] += abs(ca[1] - cb[1]);
					diff[2] += abs(ca[2] - cb[2]);
				}
			}
		}
		diff /= count;
		return diff[0] + diff[1] + diff[2];
	}
	return 0;
}

void UcvMat::ImgFillBlack(cv::Mat& a, cv::Mat& b)
{
	if (a.cols == b.cols && a.rows == b.rows)
	{
		cv::Vec3b black(0, 0, 0);
		for (int i = 0; i < a.rows; ++i)
		{
			for (int j = 0; j < b.cols; ++j)
			{
				cv::Vec3b& ca = a.at<cv::Vec3b>(i, j);
				cv::Vec3b& cb = b.at<cv::Vec3b>(i, j);
				if (ca != black && cb == black)
				{
					cb = ca;
				}
			}
		}
	}
}

float UcvMat::GetBilinearLight(float x, float y) const
{
	if (y < 0)
	{
		return 0;
	}
	if (y + 1 >= pic.rows)
	{
		return 0;
	}
	if (x < 0)
	{
		return 0;
	}
	if (x + 1 >= pic.cols)
	{
		return 0;
	}
	FVector2D left_up, left_down;
	FVector2D right_up, right_down;
	left_up.X = floor(x);
	left_up.Y = floor(y);
	left_down.X = left_up.X;
	left_down.Y = left_up.Y + 1;
	right_up.X = left_up.X + 1;
	right_up.Y = left_up.Y;
	right_down.X = left_up.X + 1;
	right_down.Y = left_up.Y + 1;
	float v[4];
	v[0] = GetLight(left_up.X, left_up.Y);
	v[1] = GetLight(left_down.X, left_down.Y);
	v[2] = GetLight(right_up.X, right_up.Y);
	v[3] = GetLight(right_down.X, right_down.Y);
	// bilinear interpolation
	float up = v[0] * abs(x - right_down.X);
	up += v[2] * abs(x - left_down.X);
	float down = v[1] * abs(x - right_up.X);
	down += v[3] * abs(x - left_up.X);
	float ans = up * abs(y - left_down.Y) + down * abs(y - left_up.Y);
	return ans;
}

float UcvMat::GetLight(int32 x, int32 y) const
{
	const cv::Vec3b& v = pic.at<cv::Vec3b>(y, x);
	return 0.299f * v[2] + 0.587f * v[1] + 0.114f * v[0];
}

floats UcvMat::GetLineLight(float x1, float y1, float x2, float y2, int32 div)
{
	floats ans;
	float step = 1.0f / (div - 1);
	FVector2D ahead(x2 - x1, y2 - y1);
	ahead *= step;
	for (int32 i = 0; i < div; ++i)
	{
		ans.Push(GetBilinearLight(x1 + ahead.X * i, y1 + ahead.Y * i));
	}
	return ans;

}

FVector UcvMat::GetBilinearColor(float x, float y) const
{
	if (y < 0)
	{
		return FVector();
	}
	if (y + 1 >= pic.rows)
	{
		return FVector();
	}
	if (x < 0)
	{
		return FVector();
	}
	if (x + 1 >= pic.cols)
	{
		return FVector();
	}
	FVector2D left_up, left_down;
	FVector2D right_up, right_down;
	left_up.X = floor(x);
	left_up.Y = floor(y);
	left_down.X = left_up.X;
	left_down.Y = left_up.Y + 1;
	right_up.X = left_up.X + 1;
	right_up.Y = left_up.Y;
	right_down.X = left_up.X + 1;
	right_down.Y = left_up.Y + 1;
	FVector v[4];
	v[0] = GetColor(left_up.X, left_up.Y);
	v[1] = GetColor(left_down.X, left_down.Y);
	v[2] = GetColor(right_up.X, right_up.Y);
	v[3] = GetColor(right_down.X, right_down.Y);
	// bilinear interpolation
	FVector up = v[0] * abs(x - right_down.X);
	up += v[2] * abs(x - left_down.X);
	FVector down = v[1] * abs(x - right_up.X);
	down += v[3] * abs(x - left_up.X);
	FVector ans = up * abs(y - left_down.Y) + down * abs(y - left_up.Y);
	return ans;
}

FVector UcvMat::GetColor(int32 x, int32 y) const
{
	const cv::Vec3b& v = pic.at<cv::Vec3b>(y, x);
	return FVector(v[0], v[1], v[2]);
}
