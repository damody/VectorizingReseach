#include "VectorizingAnimation.h"
#include "CmCurveEx.h"
#include "cvshowEX.h"

float const static PI_QUARTER = PI_FLOAT * 0.25f;
float const static PI_EIGHTH = PI_FLOAT * 0.125f;


cv::Point const DIRECTION4[4] =
{
    cv::Point(1,  0), //Direction 0
    cv::Point(0,  1), //Direction 1
    cv::Point(-1, 0), //Direction 2
    cv::Point(0, -1), //Direction 3
};  //format: {dx, dy}

cv::Point const DIRECTION8[8] =
{
    cv::Point(1,  0), //Direction 0
    cv::Point(1,  1), //Direction 1
    cv::Point(0,  1), //Direction 2
    cv::Point(-1, 1), //Direction 3
    cv::Point(-1, 0), //Direction 4
    cv::Point(-1, -1), //Direction 5
    cv::Point(0, -1), //Direction 6
    cv::Point(1, -1)  //Direction 7
};  //format: {dx, dy}

cv::Point const DIRECTION16[16] =
{
    cv::Point(2,  0), //Direction 0
    cv::Point(2,  1), //Direction 1
    cv::Point(2,  2), //Direction 2
    cv::Point(1,  2), //Direction 3
    cv::Point(0,  2), //Direction 4
    cv::Point(-1, 2), //Direction 5
    cv::Point(-2, 2), //Direction 6
    cv::Point(-2, 1), //Direction 7
    cv::Point(-2, 0), //Direction 8
    cv::Point(-2, -1), //Direction 9
    cv::Point(-2, -2), //Direction 10
    cv::Point(-1, -2), //Direction 11
    cv::Point(0, -2), //Direction 12
    cv::Point(1, -2), //Direction 13
    cv::Point(2, -2), //Direction 14
    cv::Point(2, -1)  //Direction 15
}; //format: {dx, dy}

float const DRT_ANGLE[8] =
{
    0.000000f,
    0.785398f,
    1.570796f,
    2.356194f,
    3.141593f,
    3.926991f,
    4.712389f,
    5.497787f
};

float const PI_FLOAT = 3.1415926535897932384626433832795f;
float const PI2 = PI_FLOAT * 2.0f;
float const PI_HALF = PI_FLOAT * 0.5f;


CmCurveEx::CmCurveEx(const cv::Mat& srcImg1f, float maxOrntDif)
    : m_maxAngDif(maxOrntDif)
    , m_img1f(srcImg1f)
{
    CV_Assert(srcImg1f.type() == CV_32FC1);
    m_h = m_img1f.rows;
    m_w = m_img1f.cols;
    m_pDer2.create(m_h, m_w, CV_8UC1);
    m_pDer2f.create(m_h, m_w, CV_32FC1);
    m_pOrnt1f.create(m_h, m_w, CV_32FC1);
    m_pLabel1i.create(m_h, m_w, CV_32SC1);
    m_pNext1i.create(m_h, m_w, CV_32SC1);
}

/* Compute the eigenvalues and eigenvectors of the Hessian matrix given by
dfdrr, dfdrc, and dfdcc, and sort them in descending order according to
their absolute values. */
void CmCurveEx::compute_eigenvals(double dfdrr, double dfdrc, double dfdcc,
                                  double eigval[2], double eigvec[2][2])
{
    double theta, t, c, s, e1, e2, n1, n2; /* , phi; */
    /* Compute the eigenvalues and eigenvectors of the Hessian matrix. */
    if(dfdrc != 0.0)
    {
        theta = 0.5 * (dfdcc - dfdrr) / dfdrc;
        t = 1.0 / (fabs(theta) + sqrt(theta * theta + 1.0));
        if(theta < 0.0)
        {
            t = -t;
        }
        c = 1.0 / sqrt(t * t + 1.0);
        s = t * c;
        e1 = dfdrr - t * dfdrc;
        e2 = dfdcc + t * dfdrc;
    }
    else
    {
        c = 1.0;
        s = 0.0;
        e1 = dfdrr;
        e2 = dfdcc;
    }
    n1 = c;
    n2 = -s;
    /* If the absolute value of an eigenvalue is larger than the other, put that
    eigenvalue into first position.  If both are of equal absolute value, put
    the negative one first. */
    if(fabs(e1) > fabs(e2))
    {
        eigval[0] = e1;
        eigval[1] = e2;
        eigvec[0][0] = n1;
        eigvec[0][1] = n2;
        eigvec[1][0] = -n2;
        eigvec[1][1] = n1;
    }
    else if(fabs(e1) < fabs(e2))
    {
        eigval[0] = e2;
        eigval[1] = e1;
        eigvec[0][0] = -n2;
        eigvec[0][1] = n1;
        eigvec[1][0] = n1;
        eigvec[1][1] = n2;
    }
    else
    {
        if(e1 < e2)
        {
            eigval[0] = e1;
            eigval[1] = e2;
            eigvec[0][0] = n1;
            eigvec[0][1] = n2;
            eigvec[1][0] = -n2;
            eigvec[1][1] = n1;
        }
        else
        {
            eigval[0] = e2;
            eigval[1] = e1;
            eigvec[0][0] = -n2;
            eigvec[0][1] = n1;
            eigvec[1][0] = n1;
            eigvec[1][1] = n2;
        }
    }
}

const cv::Mat& CmCurveEx::CalSecDer2(int kSize, float lineimg_threshold,
                                     float linkEndBound, float linkStartBound)
{
    cv::Mat dxMat, dyMat, blur;
    //GaussianBlur(m_img1f, blur, cv::Size(3, 3), 0, 0);
    Sobel(m_img1f, dxMat, CV_32F, 1, 0, kSize);
    Sobel(m_img1f, dyMat, CV_32F, 0, 1, kSize);
    cv::Mat dxx, dxy, dyy;
    Sobel(m_img1f, dxx, CV_32F, 2, 0, kSize);
    Sobel(m_img1f, dxy, CV_32F, 1, 1, kSize);
    Sobel(m_img1f, dyy, CV_32F, 0, 2, kSize);
    cv::Mat kx, ky;
//  getDerivKernels(kx, ky, 1, 0, 5);
//  sepFilter2D(m_img1f, dxMat, CV_32F, kx, ky, cv::Point(-1, -1), 0);
//  getDerivKernels(kx, ky, 0, 1, 5);
//  sepFilter2D(m_img1f, dyMat, CV_32F, kx, ky, cv::Point(-1, -1), 0);
//  g_cvshowEX.AddShow("dx2", dxMat);
//  g_cvshowEX.AddShow("dy2", dyMat);
    cv::Mat m_pDer1f = m_pDer2f.clone();
    double eigval[2], eigvec[2][2], maxeig = -999, mineig = 999;
    for(int y = 0; y < m_h; y++)
    {
        float* xx = dxx.ptr<float>(y);
        float* xy = dxy.ptr<float>(y);
        float* yy = dyy.ptr<float>(y);
        float* pOrnt = m_pOrnt1f.ptr<float>(y);
        float* pDer2 = m_pDer2f.ptr<float>(y);
        float* pDer1 = m_pDer1f.ptr<float>(y);
        float* dx = dxMat.ptr<float>(y);
        float* dy = dyMat.ptr<float>(y);
        for(int x = 0; x < m_w; x++)
        {
            compute_eigenvals(yy[x], xy[x], xx[x], eigval, eigvec);
            pOrnt[x] = (float)atan2(-eigvec[0][1], eigvec[0][0]); //�p��k�u��V
            pOrnt[x] = pOrnt[x] < 0 ? pOrnt[x] + PI2 : pOrnt[x];
            pDer2[x] = eigval[0];//�p��G���ɼ�
            pDer1[x] = sqrt(dx[x] * dx[x] + (dy[x] * dy[x]));
            if(pDer2[x] > maxeig)
            {
                maxeig = pDer2[x];
            }
            if(pDer2[x] < mineig)
            {
                mineig = pDer2[x];
            }
        }
    }
    //printf("min %f max %f\n", mineig, maxeig);
    normalize(m_pDer2f, m_pDer2f, -1, 1, cv::NORM_MINMAX);
    normalize(m_pDer1f, m_pDer1f, 0, 1, cv::NORM_MINMAX);
    g_cvshowEX.AddShow("m_pDer1f0", m_pDer1f);
    g_cvshowEX.AddShow("m_pDer2f0", m_pDer2f);
    GaussianBlur(m_pDer1f, m_pDer1f, cv::Size(3, 3), 0, 0);
    double sum2 = 0, avg2;
    double sum1 = 0;
    for(int y = 0; y < m_h; y++)
    {
        float* pDer2 = m_pDer2f.ptr<float>(y);
        float* pDer1 = m_pDer1f.ptr<float>(y);
        for(int x = 0; x < m_w; x++)
        {
            float v1 = pDer1[x] > 0 ? pDer1[x] + (pDer2[x]-0.5) : 0;
            float v2 = pDer2[x] > 0 ? pDer2[x] * (1 - abs(pDer1[x])) : 0;
            v2 = v2 > 0 ? v2 : 0;
            pDer1[x] = v1;
            pDer2[x] = v2;
            sum2 += pDer2[x];
            //pDer2[x] = pDer2[x] > 0 ? float(pDer2[x] > 0.0f ? pDer2[x] : 0.0f) : 0;
        }
    }
    g_cvshowEX.AddShow("fv1", m_pDer1f);
    g_cvshowEX.AddShow("fv2", m_pDer2f);
    avg2 = sum2 / (m_h * m_w);
    for(int y = 0; y < m_h; y++)
    {
        float* pDer2 = m_pDer2f.ptr<float>(y);
        for(int x = 0; x < m_w; x++)
        {
            //if (pDer2[x] <= avg2 * 1)
            {
                //pDer2[x] = powf(pDer2[x], 2);
            }
        }
    }
    g_cvshowEX.AddShow("m_pDer1f", m_pDer1f);
    g_cvshowEX.AddShow("m_pDer2f", m_pDer2f);
    double initmax = 30;
    double avg = cv::mean(m_pDer2f)[0];
//     for(; avg > lineimg_threshold; initmax*=0.5)
//     {
//         normalize(m_pDer2f, m_pDer2f, 0, initmax, cv::NORM_MINMAX);
//         cv::Scalar v = cv::mean(m_pDer2f);
//         avg = v[0];
//     }
//     for(;avg < lineimg_threshold*0.5; initmax*=2)
//     {
//         normalize(m_pDer2f, m_pDer2f, 0, initmax, cv::NORM_MINMAX);
//         cv::Scalar v = cv::mean(m_pDer2f);
//         avg = v[0];
//     }
    printf("mean %f\n", avg);
//  if (1)
//  {
//      Sobel(m_pDer2f, dxx, CV_32F, 2, 0, kSize);
//      Sobel(m_pDer2f, dxy, CV_32F, 1, 1, kSize);
//      Sobel(m_pDer2f, dyy, CV_32F, 0, 2, kSize);
//      for (int y = 0; y < m_h; y++)
//      {
//          float* xx = dxx.ptr<float>(y);
//          float* xy = dxy.ptr<float>(y);
//          float* yy = dyy.ptr<float>(y);
//          float* pOrnt = m_pOrnt1f.ptr<float>(y);
//          for (int x = 0; x < m_w; x++)
//          {
//              compute_eigenvals(yy[x], xy[x], xx[x], eigval, eigvec);
//              pOrnt[x] = (float)atan2(-eigvec[0][1], eigvec[0][0]); //�p��k�u��V
//              pOrnt[x] = pOrnt[x] < 0 ? pOrnt[x] + PI2 : pOrnt[x];
//          }
//      }
//  }
    NoneMaximalSuppress(linkEndBound, linkStartBound);
    return m_pDer2f;
}

const cv::Mat& CmCurveEx::CalSecDer(int kSize, float linkEndBound,
                                    float linkStartBound)
{
	cv::Mat dxx, dxy, dyy;
	Sobel(m_img1f, dxx, CV_32F, 2, 0, kSize);
	Sobel(m_img1f, dxy, CV_32F, 1, 1, kSize);
	Sobel(m_img1f, dyy, CV_32F, 0, 2, kSize);
	cv::Mat m_pDer1f = m_pDer2f.clone();
	double eigval[2], eigvec[2][2];
	for (int y = 0; y < m_h; y++)
	{
		float* xx = dxx.ptr<float>(y);
		float* xy = dxy.ptr<float>(y);
		float* yy = dyy.ptr<float>(y);
		float* pOrnt = m_pOrnt1f.ptr<float>(y);
		float* pDer2 = m_pDer2f.ptr<float>(y);
		float* pDer1 = m_pDer1f.ptr<float>(y);
		for (int x = 0; x < m_w; x++)
		{
			compute_eigenvals(yy[x], xy[x], xx[x], eigval, eigvec);
			pOrnt[x] = (float)atan2(-eigvec[0][1], eigvec[0][0]); //�p��k�u��V
			pOrnt[x] = pOrnt[x] < 0 ? pOrnt[x] + PI2 : pOrnt[x];

			pDer2[x] = eigval[0];//�p��G���ɼ�
		}
	}
	NoneMaximalSuppress(linkEndBound, linkStartBound);
	return m_pDer2f;
}

const cv::Mat& CmCurveEx::CalFirDer(int kSize, float linkEndBound,
                                    float linkStartBound)
{
    cv::Mat dxMat, dyMat;
    Sobel(m_img1f, dxMat, CV_32F, 1, 0, kSize);
    Sobel(m_img1f, dyMat, CV_32F, 0, 1, kSize);
    for(int y = 0; y < m_h; y++)
    {
        float* dx = dxMat.ptr<float>(y);
        float* dy = dyMat.ptr<float>(y);
        float* pOrnt = m_pOrnt1f.ptr<float>(y);
        float* pDer = m_pDer2f.ptr<float>(y);
        for(int x = 0; x < m_w; x++)
        {
            pOrnt[x] = (float)atan2(dx[x], -dy[x]);
            if(pOrnt[x] < 0.0f)
            {
                pOrnt[x] += PI2;
            }
            pDer[x] = sqrt(dx[x] * dx[x] + dy[x] * dy[x]);
        }
    }
    //GaussianBlur(m_pDer2f, m_pDer2f, cv::Size(3, 3), 0);
    //normalize(m_pDer2f, m_pDer2f, 0, 1, cv::NORM_MINMAX);
    NoneMaximalSuppress(linkEndBound, linkStartBound);
    return m_pDer2f;
}

void CmCurveEx::NoneMaximalSuppress(float linkEndBound, float linkStartBound)
{
    CV_Assert(m_pDer2f.data != NULL && m_pLabel1i.data != NULL);
    m_StartPnt.clear();
    m_StartPnt.reserve(int(0.08 * m_h * m_w));
    PntImp linePoint;
    m_pLabel1i = IND_BG;
    for(int r = 1; r < m_h - 1; r++)
    {
        float* pDer = m_pDer2f.ptr<float>(r);
        float* pOrnt = m_pOrnt1f.ptr<float>(r);
        int* pLineInd = m_pLabel1i.ptr<int>(r);
        for(int c = 1; c < m_w - 1; c++)
        {
            if(pDer[c] < linkEndBound)
            {
                continue;
            }
            float cosN = sin(pOrnt[c]);
            float sinN = -cos(pOrnt[c]);
            int xSgn = CmSgn<float>(cosN);
            int ySgn = CmSgn<float>(sinN);
            cosN *= cosN;
            sinN *= sinN;
            if(pDer[c] >= (pDer[c + xSgn] * cosN + m_pDer2f.at<float>(r + ySgn, c) * sinN)
                    && pDer[c] >= (pDer[c - xSgn] * cosN + m_pDer2f.at<float>(r - ySgn, c) * sinN))
            {
                pLineInd[c] = IND_NMS;
                if(pDer[c] < linkStartBound)
                {
                    continue;
                }
                //add to m_vStartPoint
                linePoint.second = cv::Point(c, r);
                linePoint.first = pDer[c];
                m_StartPnt.push_back(linePoint);
            }
        }
    }
}

const CEdges& CmCurveEx::Link(int shortRemoveBound /* = 3 */)
{
    CV_Assert(m_pDer2f.data != NULL && m_pLabel1i.data != NULL);
    sort(m_StartPnt.begin(), m_StartPnt.end(), linePointGreater);
    m_pNext1i = -1;
    m_vEdge.clear();
    m_vEdge.reserve(int(0.01 * m_w * m_h));
    CEdge crtEdge(0);//��e��
    for(std::vector<PntImp>::iterator it = m_StartPnt.begin();
            it != m_StartPnt.end(); it++)
    {
        cv::Point pnt = it->second;
        if(m_pLabel1i.at<int>(pnt) != IND_NMS)
        {
            continue;
        }
        findEdge(pnt, crtEdge, 0);
        findEdge(pnt, crtEdge, 1);
        if(crtEdge.pointNum <= shortRemoveBound)
        {
            cv::Point point = crtEdge.start;
            int i, nextInd;
            for(i = 1; i < crtEdge.pointNum; i++)
            {
                m_pLabel1i.at<int>(point) = IND_SR;
                nextInd = m_pNext1i.at<int>(point);
                point += DIRECTION8[nextInd];
            }
            m_pLabel1i.at<int>(point) = IND_SR;
        }
        else
        {
            m_vEdge.push_back(crtEdge);
            crtEdge.index++;
        }
    }
    // Get edge information
    int edgNum = (int)m_vEdge.size();
    for(int i = 0; i < edgNum; i++)
    {
        CEdge& edge = m_vEdge[i];
        CvLine& pnts = edge.pnts;
        pnts.resize(edge.pointNum);
        pnts[0] = edge.start;
        for(int j = 1; j < edge.pointNum; j++)
        {
            pnts[j] = pnts[j - 1] + DIRECTION8[m_pNext1i.at<int>(pnts[j - 1])];
        }
    }
    return m_vEdge;
}

/************************************************************************/
/* �p�GisForward��TRUE�h�u��m_pOrnt��V�M��crtEdge, �ñN�u�~�I��m_pNext */
/* �����ȸm���u�~����V��, �P�ɧ�m_pLineInd���ȸm����e�u���s��,�䤣��  */
/* �U�@�I���ɭԧ�̫�@���I�����иm��crtEdge��End����.                  */
/* �p�GisForward��FALSE�h�u��m_pOrnt�Ϥ�V�M��crtEdge, �ñN�u�~�I��     */
/* m_pNext�����ȸm���u�~����V�ϦV��, �P�ɧ�m_pLineInd���ȸm����e�u��  */
/* �s��.�䤣��U�@�I���ɭԦp�GpointNum�Ӥp�hactive�m��false�ñ��X.�_�h  */
/* ��̫�@���I�����иm��crtEdge��End����.                              */
/************************************************************************/
void CmCurveEx::findEdge(cv::Point seed, CEdge& crtEdge, bool isBackWard)
{
    cv::Point pnt = seed;
    float ornt = m_pOrnt1f.at<float>(pnt);
    if(isBackWard)
    {
        ornt += PI_FLOAT;
        if(ornt >= PI2)
        {
            ornt -= PI2;
        }
    }
    else
    {
        crtEdge.pointNum = 1;
        m_pLabel1i.at<int>(pnt) = crtEdge.index;
    }
    int orntInd, nextInd1, nextInd2;
    while(true)
    {
        /*************�����u���ŴM��U�@���I�A��V�t�����j���[�J**************/
        //�U�@���I�bDIRECTION16�̨Τ�V�W��
        orntInd = int(ornt / PI_EIGHTH + 0.5f) % 16;
        if(jumpNext(pnt, ornt, crtEdge, orntInd, isBackWard))
        {
            continue;
        }
        //�U�@���I�bDIRECTION8�̨Τ�V�W��
        orntInd = int(ornt / PI_QUARTER + 0.5f) % 8;
        if(goNext(pnt, ornt, crtEdge, orntInd, isBackWard))
        {
            continue;
        }
        //�U�@���I�bDIRECTION16���u��V�W��
        orntInd = int(ornt / PI_EIGHTH + 0.5f) % 16;
        nextInd1 = (orntInd + 1) % 16;
        nextInd2 = (orntInd + 15) % 16;
        if(angle(DRT_ANGLE[nextInd1], ornt) < angle(DRT_ANGLE[nextInd2], ornt))
        {
            if(jumpNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
            if(jumpNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
        }
        else //�U�@���I�bDIRECTION16�t�@�Ӥ�V�W��
        {
            if(jumpNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
            if(jumpNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
        }
        //�U�@���I�bDIRECTION8���u��V�W��
        orntInd = int(ornt / PI_QUARTER + 0.5f) % 8;
        nextInd1 = (orntInd + 1) % 8;
        nextInd2 = (orntInd + 7) % 8;
        if(angle(DRT_ANGLE[nextInd1], ornt) < angle(DRT_ANGLE[nextInd2], ornt))
        {
            if(goNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
            if(goNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
        }
        else //�U�@���I�bDIRECTION8�t�@�Ӥ�V�W��
        {
            if(goNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
            if(goNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
        }
        /*************�����u���ŴM��U�@���I�A��V�t�����j�]�[�J**************/
        //�U�@���I�bDIRECTION16�̨Τ�V�W��
        orntInd = int(ornt / PI_EIGHTH + 0.5f) % 16;
        if(jumpNext(pnt, ornt, crtEdge, orntInd, isBackWard))
        {
            continue;
        }
        //�U�@���I�bDIRECTION8�̨Τ�V�W��
        orntInd = int(ornt / PI_QUARTER + 0.5f) % 8;
        if(goNext(pnt, ornt, crtEdge, orntInd, isBackWard))
        {
            continue;
        }
        //�U�@���I�bDIRECTION16���u��V�W��
        orntInd = int(ornt / PI_EIGHTH + 0.5f) % 16;
        nextInd1 = (orntInd + 1) % 16;
        nextInd2 = (orntInd + 15) % 16;
        if(angle(DRT_ANGLE[nextInd1], ornt) < angle(DRT_ANGLE[nextInd2], ornt))
        {
            if(jumpNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
            if(jumpNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
        }
        else //�U�@���I�bDIRECTION16�t�@�Ӥ�V�W��
        {
            if(jumpNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
            if(jumpNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
        }
        //�U�@���I�bDIRECTION8���u��V�W��
        orntInd = int(ornt / PI_QUARTER + 0.5f) % 8;
        nextInd1 = (orntInd + 1) % 8;
        nextInd2 = (orntInd + 7) % 8;
        if(angle(DRT_ANGLE[nextInd1], ornt) < angle(DRT_ANGLE[nextInd2], ornt))
        {
            if(goNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
            if(goNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
        }
        else //�U�@���I�bDIRECTION8�t�@�Ӥ�V�W��
        {
            if(goNext(pnt, ornt, crtEdge, nextInd2, isBackWard))
            {
                continue;
            }
            if(goNext(pnt, ornt, crtEdge, nextInd1, isBackWard))
            {
                continue;
            }
        }
        break;//�p�Gornt���񪺤T�Ӥ�V�W���S������,�����M��
    }
    if(isBackWard)
    {
        crtEdge.start = pnt;
    }
    else
    {
        crtEdge.end = pnt;
    }
}


float CmCurveEx::angle(float ornt1, float orn2)
{
    //���ornt�������b[0, 2*PI)����, ��^�Ȧb[0, PI/2)����
    float agl = ornt1 - orn2;
    if(agl < 0)
    {
        agl += PI2;
    }
    if(agl >= PI_FLOAT)
    {
        agl -= PI_FLOAT;
    }
    if(agl >= PI_HALF)
    {
        agl -= PI_FLOAT;
    }
    return fabs(agl);
}

void CmCurveEx::refreshOrnt(float& ornt, float& newOrnt)
{
    static const float weightOld = 0.0f;
    static const float weightNew = 1.0f - weightOld;
    static const float largeBound = PI_FLOAT + PI_HALF;
    static const float smallBound = PI_HALF;
    if(newOrnt >= ornt + largeBound)
    {
        newOrnt -= PI2;
        ornt = ornt * weightOld + newOrnt * weightNew;
        if(ornt < 0.0f)
        {
            ornt += PI2;
        }
    }
    else if(newOrnt + largeBound <= ornt)
    {
        newOrnt += PI2;
        ornt = ornt * weightOld + newOrnt * weightNew;
        if(ornt >= PI2)
        {
            ornt -= PI2;
        }
    }
    else if(newOrnt >= ornt + smallBound)
    {
        newOrnt -= PI_FLOAT;
        ornt = ornt * weightOld + newOrnt * weightNew;
        if(ornt < 0.0f)
        {
            ornt += PI2;
        }
    }
    else if(newOrnt + smallBound <= ornt)
    {
        newOrnt += PI_FLOAT;
        ornt = ornt * weightOld + newOrnt * weightNew;
        if(ornt >= PI2)
        {
            ornt -= PI2;
        }
    }
    else
    {
        ornt = ornt * weightOld + newOrnt * weightNew;
    }
    newOrnt = ornt;
}

bool CmCurveEx::goNext(cv::Point& pnt, float& ornt, CEdge& crtEdge, int orntInd,
                       bool isBackward)
{
    cv::Point pntN = pnt + DIRECTION8[orntInd];
    int& label = m_pLabel1i.at<int>(pntN);
    //�p�G���I��V�P��e�u��V�t�O����j�h���[�J/***********�@�ӥi�ܰ��**********************/
    if(CHK_IND(pntN) && (label == IND_NMS || label == IND_SR))
    {
        if(angle(ornt, m_pOrnt1f.at<float>(pntN)) > m_maxAngDif)
        {
            return 0;
        }
        label = crtEdge.index;
        if(isBackward)
        {
            m_pNext1i.at<int>(pntN) = (orntInd + 4) % 8;
        }
        else
        {
            m_pNext1i.at<int>(pnt) = orntInd;
        }
        crtEdge.pointNum++;
        //��s���u��V
        refreshOrnt(ornt, m_pOrnt1f.at<float>(pntN));
        pnt = pntN;
        return true;
    }
    return false;
}

bool CmCurveEx::jumpNext(cv::Point& pnt, float& ornt, CEdge& crtEdge,
                         int orntInd, bool isBackward)
{
    cv::Point pnt2 = pnt + DIRECTION16[orntInd];
    if(CHK_IND(pnt2) && m_pLabel1i.at<int>(pnt2) <= IND_NMS)
    {
        if(angle(ornt, m_pOrnt1f.at<float>(pnt2)) >
                m_maxAngDif) //�p�G���I��V�P��e�u��V�t�O����j�h���[�J
        {
            return false;
        }
        // DIRECTION16��V�W��orntInd�۷��DIRECTION8��V�W���orntInd1,orntInd2
        // ���|�[,����orntInd = orntInd1 + orntInd2.���B�u����ܨϱo�զX�W���I��
        // IND_NMS�аO����V�զX�C(orntInd1,orntInd2�bfloor(orntInd/2)�M
        // ceil(orntInd/2)�����
        int orntInd1 = orntInd >> 1, orntInd2;
        cv::Point pnt1 = pnt + DIRECTION8[orntInd1];
        if(m_pLabel1i.at<int>(pnt1) >= IND_BG && orntInd % 2)
        {
            orntInd1 = ((orntInd + 1) >> 1) % 8;
            pnt1 = pnt + DIRECTION8[orntInd1];
        }
        int& lineIdx1 = m_pLabel1i.at<int>(pnt1);
        if(lineIdx1 != -1)  //��enPos1�I���䥦�u�W���I�A�����k�J��e�u
        {
            return false;
        }
        orntInd2 = orntInd - orntInd1;
        orntInd2 %= 8;
        lineIdx1 = crtEdge.index;
        m_pLabel1i.at<int>(pnt2) = crtEdge.index;
        if(isBackward)
        {
            m_pNext1i.at<int>(pnt1) = (orntInd1 + 4) % 8;
            m_pNext1i.at<int>(pnt2) = (orntInd2 + 4) % 8;
        }
        else
        {
            m_pNext1i.at<int>(pnt) = orntInd1;
            m_pNext1i.at<int>(pnt1) = orntInd2;
        }
        crtEdge.pointNum += 2;
        refreshOrnt(ornt, m_pOrnt1f.at<float>(pnt1));
        refreshOrnt(ornt, m_pOrnt1f.at<float>(pnt2));
        pnt = pnt2;
        return true;
    }
    return false;
}
