// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineV2.h"
#include "FrameInfo.generated.h"


class PicMesh;
/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UFrameInfo : public UObject
{
	GENERATED_BODY()
public:
	 
	TArray<ULineV2*>	BlackLine;
	
};
