#pragma once

#include "Containers/Array.h"
#include "TriangulationBase.generated.h"

USTRUCT()
struct FTriData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Vectorizing")
	FVector2D p[3];
};
typedef TArray<FTriData> FTriDatas;

USTRUCT()
struct FColorTriData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Vectorizing")
	FVector2D p[3];
	
	UPROPERTY(EditAnywhere, Category = "Vectorizing")
	FVector c[3];
};
typedef TArray<FColorTriData> FColorTriDatas;

UCLASS(Abstract)
class UTriangulationBase : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vectorizing")
	TArray<FColorTriData>   CTriDatas;
};

