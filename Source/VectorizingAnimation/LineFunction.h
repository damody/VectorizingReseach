// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LineEnd.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LineFunction.generated.h"


/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API ULineFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:	
	
	UFUNCTION(BlueprintCallable, Category = "Line")
	static TArray<ULineEnd*> LinkLineEnds180(TArray<ULineEnd*> les, float distance, float angle);

	UFUNCTION(BlueprintCallable, Category = "Line")
	static bool CheckLinkEnd_Similarity180(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

	UFUNCTION(BlueprintCallable, Category = "Line")
	static bool CheckLinkEnd_Similarity(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

	UFUNCTION(BlueprintCallable, Category = "Line")
	static TArray<UueLine*> ConnectLineEnds3(const TArray<ULineEnd*>& les, TArray<UueLine*> pos);

	FORCEINLINE static bool CheckAngle(double a1, double a2, double limitA)
	{
		if (a2 < a1)
		{
			double t = a1;
			a1 = a2;
			a2 = t;
		}
		if (abs(a1 - a2) < limitA || abs((a1 + 360) - a2) < limitA)
		{
			return true;
		}
		return false;
	}
};
