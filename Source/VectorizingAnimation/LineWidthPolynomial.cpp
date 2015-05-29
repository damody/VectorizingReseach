// Fill out your copyright notice in the Description page of Project Settings.

#include "VectorizingAnimation.h"
#include "LineWidthPolynomial.h"

ULineWidthPolynomial::ULineWidthPolynomial()
{
    Length = 0;
    NeedBuildModel = false;
}

void ULineWidthPolynomial::AddPoint(float x, float y, float w1, float w2)
{
    NeedBuildModel = true;
    Pos.Add(FVector2D(x, y));
    Widths1.Add(w1);
    Widths2.Add(w2);
    if(Pos.Num() > 1)
    {
		Length += FVector2D::Distance(Pos[Pos.Num() - 1], Pos[Pos.Num() - 2]);
    }
}

void ULineWidthPolynomial::Clear()
{
    Pos.Reset();
}

FVector2D ULineWidthPolynomial::Interpolate(float dis)
{
    float xx = x1[0] + x1[1] * dis + x1[2] * dis * dis + x1[3] * dis * dis * dis + x1[4] * dis * dis * dis * dis;
    float yy = x2[0] + x2[1] * dis + x2[2] * dis * dis + x2[3] * dis * dis * dis + x2[4] * dis * dis * dis * dis;
    return FVector2D(xx, yy);
}

void ULineWidthPolynomial::BuildModel()
{
    NeedBuildModel = false;
    const int LEN = Widths1.Num();
    const int DIM = 5;
    MatrixXd A(LEN, DIM), At, Ata;
    VectorXd b1(LEN), Atb1;
    VectorXd b2(LEN), Atb2;
    int c = 0;
	float len = 0;
    for(int i = 0; i < Widths1.Num(); ++i)
    {
        if(i != 0)
        {
			len += FVector2D::Distance(Pos[i], Pos[i - 1]);
        }
        b1.coeffRef(c) = Widths1[i];
        b2.coeffRef(c) = Widths2[i];
        A.coeffRef(c, 0) = 1;
        A.coeffRef(c, 1) = len;
		A.coeffRef(c, 2) = len * len;
		A.coeffRef(c, 3) = len * len * len;
		A.coeffRef(c, 4) = len * len * len * len;
        c++;
    }
    At = A.transpose();
    Ata = At * A;
    Atb1 = At * b1;
    Atb2 = At * b2;
    Eigen::PartialPivLU<MatrixXd> Atalu = Ata.lu();
    x1 = Atalu.solve(Atb1);
    x2 = Atalu.solve(Atb2);
}
