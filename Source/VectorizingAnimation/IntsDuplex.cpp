// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "IntsDuplex.h"

UIntsDuplex* UIntsDuplex::Clone()
{
	UIntsDuplex* res = NewObject<UIntsDuplex>();
	res->left = this->left;
	res->right = this->right;
	return res;
}

TArray<UIntsDuplex*> UIntsDuplex::CloneArray(const TArray<UIntsDuplex*>& src)
{
	TArray<UIntsDuplex*> res;
	res.Reset(src.Num());
	for (int32 i = 0; i < src.Num(); ++i)
	{
		res[i] = src[i]->Clone();
	}
	return res;
}
