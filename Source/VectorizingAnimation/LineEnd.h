// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LineEnd.generated.h"


UENUM(BlueprintType)
enum class ELinkMethod : uint8
{
	NOTHING = 0,
	BEGIN_TO_BEGIN = 0x01,
	BEGIN_TO_END = 0x02,
	END_TO_BEGIN = 0x04,
	END_TO_END = 0x08,
};

/**
 * 
 */
UCLASS()
class VECTORIZINGANIMATION_API ULineEnd : public UObject
{
	GENERATED_BODY()
public:
	ULineEnd*  linkIdx1;
	ULineEnd*  linkIdx2;
	FVector2D beg;
	FVector2D end;
	// angleBeg is p[0]-p[2]
	// angleEnd is p[last]-p[last-2]
	int32 idx, len;
	double angleBeg, angleEnd;
	ULineEnd* beglink;
	ULineEnd* endlink;
	TArray<int32> beglinks;
	TArray<int32> endlinks;
	bool haslink1, haslink2;
	ULineEnd(){}
	FORCEINLINE void Set(const FVector2D& p1, const FVector2D& p2, int32 i1, int32 i2, float a1, float a2)
	{
		beg = (p1);
		end = (p2);
		idx = (i1);
		len = (i2);
		angleBeg = (a1);
		angleEnd = (a2);
		linkIdx1 = (0);
		linkIdx2 = (0);
		beglink = (0);
		endlink = (0);
		haslink1 = (0);
		haslink2 = (0);
	}
		
};
