// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "VectorizingDataFactory.h"

#define LOCTEXT_NAMESPACE "Role"

UVectorizingDataFactory::UVectorizingDataFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
	bEditAfterNew = true;
	SupportedClass = UVectorizingData::StaticClass();
}

UObject* UVectorizingDataFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UVectorizingData* NewObjectAsset = NewObject<UVectorizingData>(InParent, Class, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE
