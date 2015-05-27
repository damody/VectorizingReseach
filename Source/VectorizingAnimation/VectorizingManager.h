// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "cvMat.h"
#include "VectorizingManager.generated.h"

UCLASS()
class VECTORIZINGANIMATION_API AVectorizingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVectorizingManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	TArray<FVector2D> Moves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mat")
	TArray<UcvMat*> Videos;
};
