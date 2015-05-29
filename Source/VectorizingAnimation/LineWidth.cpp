// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineWidth.h"
// for GEngine
#include "Engine.h"

ULineWidth::ULineWidth()
{
    Length = 0;
}

void ULineWidth::AddPoint(float x, float y, float w1, float w2)
{
    Pos.Add(FVector2D(x, y));
    Widths1.Add(w1);
    Widths2.Add(w2);
    if(Pos.Num() > 1)
    {
		Length += FVector2D::Distance(Pos[Pos.Num() - 1], Pos[Pos.Num() - 2]);
    }
}

void ULineWidth::Clear()
{
    Pos.Reset();
}

void ULineWidth::ImportLine(const ULineV2* pos, const ULineV2* width)
{
	if (width->pts.Num() != pos->pts.Num())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, "ULineWidth::ImportLine width->pts.Num() != pos->pts.Num()");
		return;
	}
	Pos = pos->pts;
	Widths1.Reset(width->pts.Num());
	Widths2.Reset(width->pts.Num());
	for (int32 i = 0; i < width->pts.Num(); ++i)
	{
		Widths1[i] = width->pts[i][0];
		Widths2[i] = width->pts[i][1];
	}
}
