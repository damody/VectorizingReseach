// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineV2Duplex.h"

void ULineV2Duplex::Move(FVector2D vec)
{
	for (int i = 0; i < left.Num(); ++i)
	{
		left[i] += vec;
	}
	for (int i = 0; i < right.Num(); ++i)
	{
		right[i] += vec;
	}
}

ULineV2Duplex* ULineV2Duplex::Clone()
{
	ULineV2Duplex* res = NewObject<ULineV2Duplex>();
	res->left = this->left;
	res->right = this->right;
	return res;
}
