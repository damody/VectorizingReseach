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

	UFUNCTION(BlueprintCallable, Category = "Line")
	UueLine*    SmoothingEach5(const UueLine* ul, float centroidRadio = 1.0f, int32 repeat = 1);
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	UueLine*    SmoothingEach3(const UueLine* ul, float centroidRadio = 1.0f, int32 repeat = 1);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (ueLine array)"), Category = "Line")
	TArray<UueLine*>   SmoothingEach5_Array(const TArray<UueLine*>& cvp, float centroidRadio = 1.0, int32 repeat = 1);
	
	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (ueLine array)"), Category = "Line")
	TArray<UueLine*>   SmoothingEach3_Array(const TArray<UueLine*>& cvp, float centroidRadio = 1.0, int32 repeat = 1);
	

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLine (FVector2D array)"), Category = "Line")
	static UueLine* GetLine(TArray<FVector2D> line);

	// bp can't call
	static UueLine* GetLine(const CvLine& line, float mx = 0.0f, float my = 0.0f);
	static TArray<UueLine*> GetLines(const CvLines& lines, float mx = 0.0f, float my = 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
	TArray<FVector2D>	pts;
		
};
