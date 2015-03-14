// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ueMat.h"
#include "RHI.h"
#include "Engine/Texture2D.h"
#include <opencv2/imgproc/imgproc.hpp>

void UueMat::SetCvMat(cv::Mat input)
{
    Pic = input.clone();
}

UueMat* UueMat::UpdateCvMatToTexture()
{
    if(Pic.cols == 0)
    {
        Pic.create(32, 32, CV_8UC3);
        for(int32 y = 0; y < Pic.rows; y++)
        {
            for(int32 x = 0; x < Pic.cols; x++)
            {
                cv::Vec3b& t = Pic.at<cv::Vec3b>(y, x);
                t[0] = rand() % 256;
                t[1] = rand() % 256;
                t[2] = rand() % 256;
            }
        }
    }
    const int SizeX = Pic.cols;
    const int SizeY = Pic.rows;
    Texture = UTexture2D::CreateTransient(SizeX, SizeY, PF_B8G8R8A8);
    FColor* MipData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
    if(Pic.type() == CV_8UC3)
    {
        for(int32 y = 0; y < Pic.rows; y++)
        {
            for(int32 x = 0; x < Pic.cols; x++)
            {
                int32 curPixelIndex = (y * Pic.cols) + x;
                FColor& c = MipData[curPixelIndex];
                cv::Vec3b& t = Pic.at<cv::Vec3b>(y, x);
                c.B = t[0];
                c.G = t[1];
                c.R = t[2];
            }
        }
    }
    else if(Pic.type() == CV_8UC1)
    {
        for(int32 y = 0; y < Pic.rows; y++)
        {
            for(int32 x = 0; x < Pic.cols; x++)
            {
                int32 curPixelIndex = (y * Pic.cols) + x;
                FColor& c = MipData[curPixelIndex];
                uchar& t = Pic.at<uchar>(y, x);
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

void UueMat::UpdateTextureToCvMat()
{
    Texture->CompressionSettings = TC_EditorIcon;
    Texture->UpdateResourceW();
    FColor* FormatedImageData = static_cast<FColor*>(Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

    Pic.create(Texture->PlatformData->Mips[0].SizeY, Texture->PlatformData->Mips[0].SizeX, CV_8UC3);
    Pic = cv::Scalar(0);
    for(int32 y = 0; y < Texture->PlatformData->Mips[0].SizeY; y++)
    {
        for(int32 x = 0; x < Texture->PlatformData->Mips[0].SizeX; x++)
        {
            int32 curPixelIndex = (y * Texture->PlatformData->Mips[0].SizeX) + x;
            FColor c = FormatedImageData[curPixelIndex];
            cv::Vec3b& t = Pic.at<cv::Vec3b>(y, x);
            t[0] = c.B;
            t[1] = c.G;
            t[2] = c.R;
        }
    }
    Texture->PlatformData->Mips[0].BulkData.Unlock();
}

UMaterialInterface* UueMat::MakeMaterial()
{
    UMaterialInstanceDynamic* RV_MatInst = UMaterialInstanceDynamic::Create(MasterMaterialRef, this);
    RV_MatInst->SetTextureParameterValue(FName("T2DParam"), Texture);
    return MasterMaterialRef;
}


EueMatEnum UueMat::GetMatState()
{
    switch(Pic.type())
    {
    case CV_8UC3:
        return EueMatEnum::UC_BGR;
    case CV_8UC1:
        return EueMatEnum::UC_Gray;
    case CV_32FC3:
        return EueMatEnum::FC_BGR;
    case CV_32FC1:
        return EueMatEnum::FC_Gray;
    }
    return EueMatEnum::Error;
}

void UueMat::ConvertMat(EueMatEnum state)
{
    switch(state)
    {
    case EueMatEnum::UC_BGR:
    {
        switch(Pic.type())
        {
        case CV_8UC3:
            break;
        case CV_8UC1:
            cvtColor(Pic.clone(), Pic, CV_GRAY2BGR);
            break;
        case CV_32FC3:
            Pic.clone().convertTo(Pic, CV_8UC1, 255);
            break;
        case CV_32FC1:
            Pic.clone().convertTo(Pic, CV_8UC1, 255);
            cvtColor(Pic.clone(), Pic, CV_GRAY2BGR);
            break;
        }
        return;
    }
    case EueMatEnum::UC_Gray:
    {
        switch(Pic.type())
        {
        case CV_8UC3:
            cvtColor(Pic.clone(), Pic, CV_BGR2GRAY);
            break;
        case CV_8UC1:
            break;
        case CV_32FC3:
            Pic.clone().convertTo(Pic, CV_8UC1, 255);
            cvtColor(Pic.clone(), Pic, CV_BGR2GRAY);
            break;
        case CV_32FC1:
            Pic.clone().convertTo(Pic, CV_8UC1, 255);
            break;
        }
        return;
    }
    case EueMatEnum::FC_BGR:
    {
        switch(Pic.type())
        {
        case CV_8UC3:
            Pic.clone().convertTo(Pic, CV_32FC1, 1.0 / 255);
            break;
        case CV_8UC1:
            Pic.clone().convertTo(Pic, CV_32FC1, 1.0 / 255);
            cvtColor(Pic.clone(), Pic, CV_GRAY2BGR);
            break;
        case CV_32FC3:
            break;
        case CV_32FC1:
            cvtColor(Pic.clone(), Pic, CV_GRAY2BGR);
            break;
        }
        return;
    }
    case EueMatEnum::FC_Gray:
    {
        switch(Pic.type())
        {
        case CV_8UC3:
            Pic.clone().convertTo(Pic, CV_32FC1, 1.0 / 255);
            cvtColor(Pic.clone(), Pic, CV_BGR2GRAY);
            break;
        case CV_8UC1:
            Pic.clone().convertTo(Pic, CV_32FC1, 1.0 / 255);
            break;
        case CV_32FC3:
            cvtColor(Pic.clone(), Pic, CV_BGR2GRAY);
            break;
        case CV_32FC1:
            break;
        }
        return;
        break;
    }
    }
}
