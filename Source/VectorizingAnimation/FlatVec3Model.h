// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "FlatVec3Model.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class VECTORIZINGANIMATION_API UFlatVec3Model : public UObject
{
	GENERATED_BODY()
public:

	virtual void AddPoint(float x, float y, FVector p) PURE_VIRTUAL(UFlatVec3Model::AddPoint, return;);
	
	virtual void BuildModel() PURE_VIRTUAL(UFlatVec3Model::BuildModel, return;);

	bool m_NeedBuildModel;

};
