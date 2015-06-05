// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LineV3.h"
#include "IntsDuplex.h"
#include "LineEnd.h"
#include "LineWidth.h"
#include "cvMat.h"
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
    static TArray<ULineEnd*> GetLineEnds(const TArray<ULineV2*>& data);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<ULineEnd*> LinkLineEnds180(TArray<ULineEnd*> les, float distance, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static bool CheckLinkEnd_Similarity180(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static bool CheckLinkEnd_Similarity(const ULineEnd* lhs, const ULineEnd* rhs, ELinkMethod c, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<ULineV2*> ConnectLineEnds3(const TArray<ULineEnd*>& les, TArray<ULineV2*> pos);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<ULineV2*> ConnectNearestLines(const TArray<ULineEnd*>& les, TArray<ULineV2*> pos, float d2, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static FVector2D GetRotation(const FVector2D& src, float angle, const FVector2D& middle = FVector2D::ZeroVector);


    // smoothing part

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D>   SmoothingEach5(const TArray<FVector2D>& data, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D>   SmoothingEach3(const TArray<FVector2D>& data, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (LineV2)"), Category = "Line")
    static ULineV2 *    SmoothingEach5_LineV2(const ULineV2* ul, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (LineV2)"), Category = "Line")
    static ULineV2 *    SmoothingEach3_LineV2(const ULineV2* ul, float centroidRadio = 1.0f, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5 (LineV2 array)"), Category = "Line")
    static TArray<ULineV2*>   SmoothingEach5_Array(const TArray<ULineV2*>& data, float centroidRadio = 1.0, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach3 (LineV2 array)"), Category = "Line")
    static TArray<ULineV2*>   SmoothingEach3_Array(const TArray<ULineV2*>& data, float centroidRadio = 1.0, int32 repeat = 1);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "SmoothingEach5Skip0 (Vector2D array)"), Category = "Line")
	static TArray<FVector2D>   SmoothingEach5Skip0(const TArray<FVector2D>& data, double centroidRadio = 1.0, int32 repeat = 1);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (Vector2D array)"), Category = "Line")
    static TArray<FVector2D> GetNormalsLen2(TArray<FVector2D> data);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (LineV2)"), Category = "Line")
    static ULineV2 * GetNormalsLen2_LineV2(const ULineV2* ul);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetNormalsLen2 (LineV2 array)"), Category = "Line")
    static TArray<ULineV2*> GetNormalsLen2_Array(TArray<ULineV2*> data);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToAngle(const TArray<float>& data, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToSquareWave(const TArray<float>& data, int32 error, float val = 100, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<ULineV2*> BuildDecorativeCurve(const TArray<ULineV2*>& line, UcvMat* umat, float blackRadio);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static TArray<float> ConvertToLineWidth(const TArray<float>& data, float lineWidth, float zero = 290);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static void ClearLineWidthByPercent(TArray<ULineV2*>& widths, float angle);

    UFUNCTION(BlueprintCallable, Category = "Line")
    static float  GetLineWidthPercent(const TArray<FVector2D>& data);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (Vector2D array)"), Category = "Line")
    static TArray<FVector2D> FixWidthLine(const TArray<FVector2D>& aLine, int32 range);

    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (LineV2)"), Category = "Line")
    static ULineV2 * FixWidthLine_LineV2(const ULineV2* uline, int32 range);
	
    UFUNCTION(BlueprintCallable, meta = (FriendlyName = "FixWidthLine (LineV2 Array)"), Category = "Line")
    static TArray<ULineV2*> FixWidthLine_Array(const TArray<ULineV2*>& data, int32 range);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLinesColor (LineV2)"), Category = "Line")
	static ULineV3* GetLinesColor_LineV2(UcvMat* img, const ULineV2* lines);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLinesColor (LineV2 Array)"), Category = "Line")
	static TArray<ULineV3*> GetLinesColor_Array(UcvMat* img, const TArray<ULineV2*>& lines);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLineWidth (LineV2)"), Category = "Line")
	static ULineWidth* GetLineWidth(const ULineV2* lines, const ULineV2* lws);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetLineWidth (LineV2 Array)"), Category = "Line")
	static TArray<ULineWidth*> GetLineWidth_Array(const TArray<ULineV2*>& lines, const TArray<ULineV2*>& lws);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "MakeBoundaryLines (LineV2)"), Category = "Line")
	static ULineV2 * MakeBoundaryLines_LineV2(UcvMat* img);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "GetIndexDuplexFromCvMat (LineV2 Array)"), Category = "Line")
	static TArray<UIntsDuplex*> GetIndexDuplexFromCvMat(TArray<ULineV2*> lines, UcvMat* img);

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
