// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ueLine.h"
#include "FrameInfo.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UFrameInfo : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<UueLine*>	BlackLine;
	
};
