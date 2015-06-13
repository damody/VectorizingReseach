// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "cvMat.h"
#include "cvMatThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

class FMyCodeProjectModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule()
	{
		UThumbnailManager::Get().RegisterCustomRenderer(UcvMat::StaticClass(), UcvMatThumbnailRenderer::StaticClass());
	}
	virtual void ShutdownModule()
	{
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FMyCodeProjectModule, VectorizingAnimation, "VectorizingAnimation");

DEFINE_LOG_CATEGORY(Vectorizing);
