// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineDef.h"
#include "LineV3.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineV3 : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Line")
	void Move(FVector vec);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	ULineV3* Clone();


	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetLine (FVector array to LineV3)"), Category = "Line")
	static ULineV3* GetLine_Array(TArray<FVector> line);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetLine (FVector array to FVector array)"), Category = "Line")
	static TArray<FVector> GetLine_FV3Array(TArray<FVector> line, float mx = 0.0f, float my = 0.0f, float mz = 0.0f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector>	pts;
	
	operator TArray<FVector>()
	{
		return pts;
	}

	FVector& operator[](int32 i)
	{
		return pts[i];
	}
};
