#pragma once

#include "Object.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "CmCurveEx.h"
#include "LineV2.h"
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
	bool SetPicture(UcvMat* umat);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	TArray<ULineV2*> CalSecDer(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	TArray<ULineV2*> CalSecDer2(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UFUNCTION(BlueprintCallable, Category = "OpenCV|CurveExtraction")
	TArray<ULineV2*> CalFirDer(int32 maskSize, float linkEndBound = 1.f, float linkStartBound = 0.05f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurveExtraction")
	TArray<ULineV2*>	Curves;

	CmCurveEx m_CmCurveEx;

	cv::Mat	m_CurvePic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CurveExtraction")
	UcvMat* m_CurveLink;
};
