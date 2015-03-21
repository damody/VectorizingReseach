// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "ueLine.h"

void UueLine::Move(FVector2D vec)
{
	for (int i = 0; i < pts.Num(); ++i)
	{
		pts[i] += vec;
	}
}

UueLine* UueLine::GetLine(const CvLine& line, float mx, float my)
{
	UueLine* res = NewObject<UueLine>();
	for (CvLine::const_iterator it = line.begin(); it != line.end(); ++it)
	{
		res->pts.Add(FVector2D(it->x + mx, it->y + my));
	}
	return res;
}

UueLine* UueLine::GetLine(TArray<FVector2D> line)
{
	UueLine* res = NewObject<UueLine>();
	res->pts = line;
	return res;
}


TArray<UueLine*> UueLine::GetLines(const CvLines& lines, float mx, float my)
{
	TArray<UueLine*> res;
	for (int i = 0; i < lines.size(); ++i)
	{
		res.Push(GetLine(lines[i]));
	}
	return res;
}

UueLine* UueLine::Clone()
{
	UueLine* res = NewObject<UueLine>();
	res->pts = this->pts;
	return res;
}

UueLine* UueLine::SmoothingEach5(const UueLine* ul, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
	const TArray<FVector2D>& cvp = ul->pts;
	if (cvp.Num() <= 2)
	{
		UueLine* res = NewObject<UueLine>();
		res->pts = ul->pts;
		return res;
	}
	TArray<FVector2D> cps = cvp;
	TArray<FVector2D> newcps;
	TArray<FVector2D> centroids;
	for (int repeatCount = 0; repeatCount < repeat; repeatCount++)
	{
		newcps.Reset();
		if (cps.Num() <= 5)
		{
			newcps.Add(cps[0]);
			for (int j = 1; j < cps.Num() - 1; j++)
			{
				auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1]) * 0.25;
				newcps.Add(vec);
			}
			newcps.Add(cps.Last());
			cps = newcps;
		}
		else
		{
			centroids.Reset();
			newcps.Add(cps[0]);
			centroids.Add((cps[0] + cps[1] + cps[2]) / 3.0f);
			newcps.Add((cps[0] + cps[1] * 2 + cps[2]) * 0.25);
			for (int j = 2; j < cps.Num() - 2; j++)
			{
				auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1] + cps[j + 2] + cps[j - 2]) /
					6.0f;
				newcps.Add(vec);
				centroids.Add((cps[j] + cps[j + 1] + cps[j - 1] + cps[j + 2] +
					cps[j - 2]) / 5.0f);
			}
			int last = (int)cps.Num() - 1;
			newcps.Add((cps[last] + cps[last - 1] * 2 + cps[last - 2]) * 0.25);
			centroids.Add((cps[last] + cps[last - 1] + cps[last - 2]) / 3.0f);
			newcps.Add(cps.Last());
			cps = newcps;
			// move centroid
			newcps.Reset();
			newcps.Add(cps[0]);
			FVector2D cert = (cps[0] + cps[1] + cps[2]) / 3.0f;
			cert = centroids[0] - cert;
			newcps.Add(cps[1] + cert);
			for (int j = 2; j < (int)cps.Num() - 2; j++)
			{
				FVector2D nowCentroid = (cps[j] + cps[j + 1] + cps[j - 1] + cps[j + 2] +
					cps[j - 2]) / 5.0f;
				nowCentroid = centroids[j - 1] - nowCentroid;
				newcps.Add(cps[j] + nowCentroid * centroidRadio);
			}
			cert = (cps[last] + cps[last - 1] + cps[last - 2]) / 3.0f;
			cert = centroids[last - 2] - cert;
			newcps.Add(cps[last - 1] + cert);
			newcps.Add(cps.Last());
			cps = newcps;
		}
	}
	UueLine* res = NewObject<UueLine>();
	res->pts = cps;
	return res;
}

TArray<UueLine*> UueLine::SmoothingEach5_Array(const TArray<UueLine*>& cvp, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
	TArray<UueLine*> ans;
	ans.SetNum(cvp.Num());
	for (int i = 0; i < cvp.Num(); ++i)
	{
		const UueLine* aa = cvp[i];
		ans[i] = SmoothingEach5(aa, centroidRadio, repeat);
	}
	return ans;
}

UueLine* UueLine::SmoothingEach3(const UueLine* ul, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
	const TArray<FVector2D>& cvp = ul->pts;
	if (cvp.Num() <= 2)
	{
		UueLine* res = NewObject<UueLine>();
		res->pts = ul->pts;
		return res;
	}
	
	TArray<FVector2D> cps = cvp;
	TArray<FVector2D> newcps;
	for (int repeatCount = 0; repeatCount < repeat; repeatCount++)
	{
		newcps.Reset();
		if (cps.Num() <= 5)
		{
			newcps.Add(cps[0]);
			for (int j = 1; j < cps.Num() - 1; j++)
			{
				auto vec = (cps[j] * 2 + cps[j + 1] + cps[j - 1]) * 0.25;
				newcps.Add(vec);
			}
			newcps.Add(cps.Last());
			cps = newcps;
		}
		else
		{
			newcps.Add(cps[0] * 0.8);
			for (int j = 2; j < cps.Num() - 2; j++)
			{
				if (cps[j].X > 0)
				{
					int zero = 0;
					for (int k = j - 1; k <= j + 1; ++k)
					{
						if (cps[k].X < 0.01)
						{
							zero++;
						}
					}
					auto vec = (cps[j] * (2 + zero) + cps[j + 1] + cps[j - 1]) / 4.0f;
					newcps.Add(vec);
				}
				else
				{
					newcps.Add(FVector2D());
				}
			}
			newcps.Add(cps.Last() * 0.8);
			cps = newcps;
		}
	}
	UueLine* res = NewObject<UueLine>();
	res->pts = cps;
	return res;
}

TArray<UueLine*> UueLine::SmoothingEach3_Array(const TArray<UueLine*>& cvp, float centroidRadio /*= 1.0*/, int32 repeat /*= 1*/)
{
	TArray<UueLine*> ans;
	ans.SetNum(cvp.Num());
	for (int i = 0; i < cvp.Num(); ++i)
	{
		const UueLine* aa = cvp[i];
		ans[i] = SmoothingEach3(aa, centroidRadio, repeat);
	}
	return ans;
}

