// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineDef.h"
#include "ueLine.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UueLine : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Line")
	void Move(FVector2D vec);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	UueLine* Clone();


	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLine (FVector2D array to UueLine)"), Category = "Line")
	static UueLine* GetLine_Array(TArray<FVector2D> line);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLine (FVector2D array to FVector2D array)"), Category = "Line")
	static TArray<FVector2D> GetLine_FV2Array(TArray<FVector2D> line, float mx = 0.0f, float my = 0.0f);


	// bp can't call
	static UueLine* GetLine(const CvLine& line, float mx = 0.0f, float my = 0.0f);
	static TArray<UueLine*> GetLines(const CvLines& lines, float mx = 0.0f, float my = 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	pts;
	
	operator TArray<FVector2D>()
	{
		return pts;
	}

	FVector2D& operator[](int32 i)
	{
		return pts[i];
	}
};
