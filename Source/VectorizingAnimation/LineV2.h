// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineDef.h"
#include "LineV2.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API ULineV2 : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Line")
	void Move(FVector2D vec);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	ULineV2* Clone();

	UFUNCTION(BlueprintCallable, Category = "Line")
	static TArray<ULineV2*> CloneArray(const TArray<ULineV2*>& src);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetLine (FVector2D array to LineV2)"), Category = "Line")
	static ULineV2* GetLine_Array(TArray<FVector2D> line);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetLine (FVector2D array to FVector2D array)"), Category = "Line")
	static TArray<FVector2D> GetLine_FV2Array(TArray<FVector2D> line, float mx = 0.0f, float my = 0.0f);


	// bp can't call
	static ULineV2* GetLine(const CvLine& line, float mx = 0.0f, float my = 0.0f);
	static TArray<ULineV2*> GetLines(const CvLines& lines, float mx = 0.0f, float my = 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	pts;
	
	operator TArray<FVector2D>()
	{
		return pts;
	}

	int32 Num() const
	{
		return pts.Num();
	}

	FVector2D& operator[](int32 i)
	{
		return pts[i];
	}
};
