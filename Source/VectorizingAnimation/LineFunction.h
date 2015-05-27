// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LineEnd.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ueMat.h"
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
    static TArray<ULineEnd*> GetLineEnds(const TArray<UueLine*>& data);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<ULineEnd*> LinkLineEnds180(TArray<ULineEnd*> les, float distance, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static bool CheckLinkEnd_Similarity180(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static bool CheckLinkEnd_Similarity(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<UueLine*> ConnectLineEnds3(const TArray<ULineEnd*>& les, TArray<UueLine*> pos);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<UueLine*> ConnectNearestLines(const TArray<ULineEnd*>& les, TArray<UueLine*> pos, float d2, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static FVector2D GetRotation(const FVector2D& src, float angle, const FVector2D& middle = FVector2D::ZeroVector);


    // smoothing part

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D>   SmoothingEach5(const TArray<FVector2D>& data, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D>   SmoothingEach3(const TArray<FVector2D>& data, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (ueLine)"), Category = "Line")
    static UueLine *    SmoothingEach5_ueLine(const UueLine* ul, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (ueLine)"), Category = "Line")
    static UueLine *    SmoothingEach3_ueLine(const UueLine* ul, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (ueLine array)"), Category = "Line")
    static TArray<UueLine*>   SmoothingEach5_Array(const TArray<UueLine*>& data, float centroidRadio = 1.0, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (ueLine array)"), Category = "Line")
    static TArray<UueLine*>   SmoothingEach3_Array(const TArray<UueLine*>& data, float centroidRadio = 1.0, int32 repeat = 1);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5Skip0 (Vector2D array)"), Category = "Line")
	static TArray<FVector2D>   SmoothingEach5Skip0(const TArray<FVector2D>& data, double centroidRadio = 1.0, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D> GetNormalsLen2(TArray<FVector2D> data);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (ueLine)"), Category = "Line")
    static UueLine * GetNormalsLen2_ueLine(const UueLine* ul);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (ueLine array)"), Category = "Line")
    static TArray<UueLine*> GetNormalsLen2_Array(TArray<UueLine*> data);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToAngle(const TArray<float>& data, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToSquareWave(const TArray<float>& data, int32 error, float val = 100, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<UueLine*> BuildDecorativeCurve(const TArray<UueLine*>& line, UueMat* umat, float blackRadio);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToLineWidth(const TArray<float>& data, float lineWidth, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static void ClearLineWidthByPercent(TArray<UueLine*>& widths, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static float  GetLineWidthPercent(const TArray<FVector2D>& data);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (Vector2D array)"), Category = "Line")
    static TArray<FVector2D> FixWidthLine(const TArray<FVector2D>& aLine, int32 range);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (ueLine)"), Category = "Line")
    static UueLine * FixWidthLine_ueLine(const UueLine* uline, int32 range);


    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (ueLine Array)"), Category = "Line")
    static TArray<UueLine*> FixWidthLine_Array(const TArray<UueLine*>& data, int32 range);

    FORCEINLINE static bool CheckAngle(float a1, float a2, float limitA)
    {
        if(a2 < a1)
        {
            double t = a1;
            a1 = a2;
            a2 = t;
        }
        if(abs(a1 - a2) < limitA || abs((a1 + 360) - a2) < limitA)
        {
            return true;
        }
        return false;
    }
};
