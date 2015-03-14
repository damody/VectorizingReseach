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
	UueMat* CalSecDer(int32 maskSize, double linkEndBound, double linkStartBound);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurveExtraction")
	TArray<UueLine*>	Curves;

	CmCurveEx m_CmCurveEx;
	cv::Mat	m_CurvePic;
};
