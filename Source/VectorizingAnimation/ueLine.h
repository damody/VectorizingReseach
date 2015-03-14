// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "ueLine.generated.h"

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API UueLine : public UObject
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	Points;
	
};
