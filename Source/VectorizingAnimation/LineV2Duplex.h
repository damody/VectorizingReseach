// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineV2Duplex.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineV2Duplex : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Line")
	void Move(FVector2D vec);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	ULineV2Duplex* Clone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	left;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	right;
	
	TArray<FVector2D>& operator[](int32 i)
	{
		if (i % 2 == 0)
		{
			return left;
		}
		else
		{
			return right;
		}
	}
};
