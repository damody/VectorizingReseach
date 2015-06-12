// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "cvMatFactory.h"

#define LOCTEXT_NAMESPACE "Role"

UcvMatFactory::UcvMatFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UcvMat::StaticClass();
}

UObject* UcvMatFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UcvMat* NewObjectAsset = NewObject<UcvMat>(InParent, Class, Name, Flags | RF_Transactional);
	return NewObjectAsset;
}

#undef LOCTEXT_NAMESPACE
