// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/**
 *
 * This thumbnail renderer displays the texture for the object in question
 */

#pragma once
#include "ThumbnailRendering/ThumbnailRenderer.h"
#include "ThumbnailRendering/DefaultSizedThumbnailRenderer.h"
#include "cvMatThumbnailRenderer.generated.h"

UCLASS(MinimalAPI)
class UcvMatThumbnailRenderer : public UThumbnailRenderer
{
	GENERATED_UCLASS_BODY()


	// Begin UThumbnailRenderer Object
	VECTORIZINGANIMATION_API virtual void GetThumbnailSize(UObject* Object, float Zoom, uint32& OutWidth, uint32& OutHeight) const override;
	VECTORIZINGANIMATION_API virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas) override;
	// End UThumbnailRenderer Object
};

