// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "MyActor.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AMyActor::imshow()
{
	MyTexture2D->CompressionSettings = TC_EditorIcon;
	MyTexture2D->UpdateResourceW();
	FColor* FormatedImageData = static_cast<FColor*>(MyTexture2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));
	cv::Mat tmp;
	tmp.create(MyTexture2D->PlatformData->Mips[0].SizeY, MyTexture2D->PlatformData->Mips[0].SizeX, CV_8UC3);
	tmp = cv::Scalar(0);
	for (int32 y = 0; y < MyTexture2D->PlatformData->Mips[0].SizeY; y++)
	{
		for (int32 x = 0; x < MyTexture2D->PlatformData->Mips[0].SizeX; x++)
		{
			int32 curPixelIndex = (y * MyTexture2D->PlatformData->Mips[0].SizeX) + x;
			FColor c = FormatedImageData[curPixelIndex];
			cv::Vec3b& t = tmp.at<cv::Vec3b>(y, x);
			t[0] = c.B;
			t[1] = c.G;
			t[2] = c.R;
		}
	}
	MyTexture2D->PlatformData->Mips[0].BulkData.Unlock();
	
	cv::imshow("tmp", tmp);
	cv::waitKey(10);
}

