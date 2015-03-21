#pragma once

#include "Object.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CmCurveEx.h"
#include "ueLine.h"
#include "CurveExtraction.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class VECTORIZINGANIMATION_API UCurveExtraction : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	static UCurveExtraction* NewCurveExtraction();

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	bool Set_ueMat(UueMat* umat);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	UueMat* CalSecDer(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	UueMat* CalSecDer2(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	UueMat* CalFirDer(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurveExtraction")
	TArray<UueLine*>	Curves;

	CmCurveEx m_CmCurveEx;
	cv::Mat	m_CurvePic;
};