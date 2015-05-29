// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

FORCEINLINE float GetLight(const FVector& v)
{
	return v[0] * 0.299f + v[1] * 0.587f + v[2] * 0.114f;
}

FORCEINLINE bool LightCompareVector3(const FVector& v1, const FVector& v2)
{
	return GetLight(v1) < GetLight(v2);
}
