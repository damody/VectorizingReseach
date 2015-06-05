// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <opencv2/highgui/highgui.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
};
#include <string>
#include "VideoReader.generated.h"

UCLASS()
class VECTORIZINGANIMATION_API AVideoReader : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVideoReader();

	~AVideoReader();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Video")
	bool Load(FString path);

	UFUNCTION(BlueprintCallable, Category = "Video")
	UcvMat* Read();

	UFUNCTION(BlueprintCallable, Category = "Video")
	bool isReadOk();

	UFUNCTION(BlueprintCallable, Category = "Video")
	int32 FrameCount();

	UFUNCTION(BlueprintCallable, Category = "Video")
	int32 CurrentFrame();
	
	cv::VideoCapture	cvVideo;
private:
	bool Open(std::string ss);
	void Close();
	cv::Mat _GetFrame(AVFrame *pFrame, int32 width, int32 height);
	bool	m_HasRead;
	std::string m_Path;
	AVFormatContext *m_pFormatCtx;
	int32             m_videoStream;
	AVCodecContext  *m_pCodecCtx;
	AVCodec         *m_pCodec;
	AVFrame         *m_pFrame;
	AVFrame         *m_pFrameRGB;
	AVPacket        m_packet;
	int32             m_frameFinished;
	int32             m_numBytes;
	int32				m_frameCount;
	int32				m_current;
	uint8_t         *m_buffer;
};
