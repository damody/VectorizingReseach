// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "IntsDuplex.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UIntsDuplex : public UObject
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category = "Line")
	UIntsDuplex* Clone();

	UFUNCTION(BlueprintCallable, Category = "Line")
	static TArray<UIntsDuplex*> CloneArray(const TArray<UIntsDuplex*>& src);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<int32>	left;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<int32>	right;
	
	TArray<int32>& operator[](int32 i)
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
