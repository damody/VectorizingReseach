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
	bool res;
	IsActorInJunction(this, res);
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

AMyActor::AMyActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	DesiredIntensity = 3000.0f;

	PointLight1 = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, "PointLight1");
	PointLight1->Intensity = DesiredIntensity;
	PointLight1->bVisible = true;
	RootComponent = PointLight1;

	Sphere1 = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere1"));
	Sphere1->InitSphereRadius(250.0f);
	Sphere1->AttachParent = RootComponent;

	Sphere1->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::OnOverlapBegin);       // set up a notification for when this component overlaps something
	Sphere1->OnComponentEndOverlap.AddDynamic(this, &AMyActor::OnOverlapEnd);       // set up a notification for when this component overlaps something
}

void AMyActor::OnOverlapBegin_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ToggleLight();
	}
}

void AMyActor::OnOverlapEnd_Implementation(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		ToggleLight();
	}
}

void AMyActor::ToggleLight()
{
	PointLight1->ToggleVisibility();
}

