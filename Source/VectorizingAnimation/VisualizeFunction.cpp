// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "VisualizeFunction.h"

void UVisualizeFunction::DrawLines(FVector center, TArray<ULineV2*> lines)
{
    for(int32 i = 0; i < lines.Num(); ++i)
    {
        ULineV2& nowline = *lines[i];
        for(int32 j = 0; j < nowline.Num() - 1; ++j)
        {
            FVector LinkStart = center, LinkEnd = center;
            LinkStart.X += nowline[j].X;
            LinkStart.Y += nowline[j].Y;
            LinkEnd.X += nowline[j + 1].X;
            LinkEnd.Y += nowline[j + 1].Y;
//             DrawDebugLine(
//                 GetWorld(),
//                 LinkStart,
//                 LinkEnd,
//                 FColor(255, 0, 0),
//                 false, -1, 0,
//                 12.333
//             );
        }
    }

}


void UVisualizeFunction::ImShow(FString name, UcvMat* img)
{
    cv::imshow(TCHAR_TO_ANSI(*name), img->pic);
    cv::waitKey(50);
}

void UVisualizeFunction::DestroyAllImShow()
{
	cv::destroyAllWindows();
}

void UVisualizeFunction::DestroyImShow(FString name)
{
	cv::destroyWindow(TCHAR_TO_ANSI(*name));
}

