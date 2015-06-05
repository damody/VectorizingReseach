// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "VideoReader.h"
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvMat.h"
// for GEngine
#include "Engine.h"
// for convert path
#include "ConvStr.h"

// Sets default values
AVideoReader::AVideoReader()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	m_HasRead = false;
	m_current = 0;
	m_frameCount = 0;
}

// Called when the game starts or when spawned
void AVideoReader::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AVideoReader::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


bool AVideoReader::Open(std::string ss)
{
	m_pFormatCtx = NULL;
	av_register_all();
	if (avformat_open_input(&m_pFormatCtx, ss.c_str(), NULL, NULL) != 0)
	{
		return false;
	}
	if (avformat_find_stream_info(m_pFormatCtx, NULL) < 0)
	{
		return false;
	}
	av_dump_format(m_pFormatCtx, -1, ss.c_str(), 0);
	m_videoStream = -1;
	for (int32 i = 0; i < (int32)m_pFormatCtx->nb_streams; i++)
	{
		if (m_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			m_videoStream = i;
			break;
		}
	}
	if (m_videoStream == -1)
	{
		return false;
	}
	m_pCodecCtx = m_pFormatCtx->streams[m_videoStream]->codec;
	m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
	if (m_pCodec == NULL)
	{
		return false;
	}
	if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
	{
		return false;
	}
	m_pFrame = avcodec_alloc_frame();
	if (m_pFrame == NULL)
	{
		return false;
	}
	m_pFrameRGB = avcodec_alloc_frame();
	if (m_pFrameRGB == NULL)
	{
		return false;
	}
	m_numBytes = avpicture_get_size(PIX_FMT_RGB24, m_pCodecCtx->width,
		m_pCodecCtx->height);
	m_buffer = (uint8_t*)av_malloc(m_numBytes);
	avpicture_fill((AVPicture*)m_pFrameRGB, m_buffer, PIX_FMT_RGB24,
		m_pCodecCtx->width, m_pCodecCtx->height);
	m_HasRead = true;
	return true;
}

bool AVideoReader::Load(FString path)
{
    std::string ss = ConvStr::GetStr(*path);
	if (Open(ss))
	{
		m_frameCount = 0;
		while (av_read_frame(m_pFormatCtx, &m_packet) >= 0)
		{
			if (m_packet.stream_index == m_videoStream)
			{
				avcodec_decode_video2(m_pCodecCtx, m_pFrame, &m_frameFinished, &m_packet);
				if (m_frameFinished)
				{
					m_frameCount++;
				}
			}
			av_free_packet(&m_packet);
		}
		avformat_close_input(&m_pFormatCtx);
		if (avformat_open_input(&m_pFormatCtx, ss.c_str(), NULL, NULL) != 0)
		{
			return false;
		}
		char buff[100];
		sprintf(buff, "FrameCount %d", m_frameCount);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, buff);
	}
	else
	{
		return false;
	}
	Close();
	m_current = 0;
	Open(ss);
	return true;
}

UcvMat* AVideoReader::Read()
{
    UcvMat* umat = NewObject<UcvMat>();
	cv::Mat res;
	while (av_read_frame(m_pFormatCtx, &m_packet) >= 0)
	{
		if (m_packet.stream_index == m_videoStream)
		{
			avcodec_decode_video2(m_pCodecCtx, m_pFrame, &m_frameFinished, &m_packet);
			if (m_frameFinished)
			{
				struct SwsContext* img_convert_ctx = NULL;
				img_convert_ctx =
					sws_getCachedContext(img_convert_ctx, m_pCodecCtx->width,
					m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
					m_pCodecCtx->width, m_pCodecCtx->height,
					PIX_FMT_RGB24, SWS_BICUBIC,
					NULL, NULL, NULL);
				if (!img_convert_ctx)
				{
					fprintf(stderr, "Cannot initialize sws conversion context\n");
				}
				sws_scale(img_convert_ctx, (const uint8_t * const*)m_pFrame->data,
					m_pFrame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data,
					m_pFrameRGB->linesize);
				res = _GetFrame(m_pFrameRGB, m_pCodecCtx->width, m_pCodecCtx->height);
				av_free_packet(&m_packet);
				umat->Copy(res);
				m_current++;
				return umat;
			}
		}
		av_free_packet(&m_packet);
	}
	CurrentFrame();
    return umat;
}

bool AVideoReader::isReadOk()
{
	return m_HasRead && m_current < m_frameCount;
}

int32 AVideoReader::FrameCount()
{
	return m_frameCount;
}

int32 AVideoReader::CurrentFrame()
{
    int32 a = (int32)cvVideo.get(CV_CAP_PROP_POS_FRAMES);
    char buff[100];
    sprintf(buff, "CurrentFrame %d", a);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, buff);
    return (int32)cvVideo.get(CV_CAP_PROP_POS_FRAMES);
}

cv::Mat AVideoReader::_GetFrame(AVFrame *pFrame, int32 width, int32 height)
{
	cv::Mat res;
	res.create(height, width, CV_8UC3);
	uint8_t* t_data = new uint8_t[width * 3];
	for (int32 y = 0; y < height; y++)
	{
		memcpy(t_data, pFrame->data[0] + y * pFrame->linesize[0], width * 3);
		for (int32 i = 0; i < width * 3; i += 3)
		{
			cv::Vec3b& c = res.at<cv::Vec3b>(y, i / 3);
			c[0] = t_data[i + 2];
			c[1] = t_data[i + 1];
			c[2] = t_data[i];
		}
	}
	delete t_data;
	return res;
}

AVideoReader::~AVideoReader()
{
	Close();
	avcodec_close(m_pCodecCtx);
}

void AVideoReader::Close()
{
	if (m_HasRead)
	{
		avformat_close_input(&m_pFormatCtx);
		av_free(m_buffer);
		av_free(m_pFrameRGB);
		av_free(m_pFrame);
	}
}

