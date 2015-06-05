#include "VectorizingAnimation.h"
#include "cvshowEX.h"
#include "ConvStr.h"
#include <xutility>


const int32 CHECK_1 = 1;
HWND g_hwnd2;
LONG_PTR g_ori_wproc2;
cvshowEX g_cvshowEX;

LRESULT CALLBACK WndProc2(HWND hwnd, uint32 msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		for (int32 i = 0; i < g_cvshowEX.m_hwnd_checks.size(); ++i)
		{
			BOOL checked = IsDlgButtonChecked(hwnd, i + 1);
//          int32 gg = GetLastError();
//          printf("gg %d\n", gg);
			if (checked)
			{
				g_cvshowEX.m_ischecks[i] = 1;
				CheckDlgButton(g_cvshowEX.m_hwnd_checks[i], i + 1, BST_UNCHECKED);
			}
			else
			{
				g_cvshowEX.m_ischecks[i] = 0;
				CheckDlgButton(g_cvshowEX.m_hwnd_checks[i], i + 1, BST_CHECKED);
				//SetDlgItemText(hwnd, CHECK_1, L"WTF");
			}
		}
		g_cvshowEX.Render();
		break;
	}
	}
	return CallWindowProc((WNDPROC)g_ori_wproc2, hwnd, msg, wParam, lParam);;
}

LONG_PTR g_my_wproc2 = (LONG_PTR)WndProc2;

cvshowEX::cvshowEX(void)
{
}


cvshowEX::~cvshowEX(void)
{
}


void cvshowEX::AddShow(std::string name, cv::Mat mat)
{
	strings::iterator it = std::find(m_names.begin(), m_names.end(), name);
	if (it == m_names.end())
	{
		if (m_num_check == m_hwnd_checks.size())
		{
			HWND hwnd = CreateWindow((L"BUTTON"), ConvStr::GetWstr(name).c_str(),
									 WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
									 w, 20 * m_num_check, 200, 20,
									 g_hwnd2, (HMENU)(m_num_check + 1), NULL, NULL);
			m_hwnd_checks.push_back(hwnd);
			m_num_check++;
			m_ischecks.push_back(0);
		}
		m_names.push_back(name);
		m_mats.push_back(mat.clone());
		if (m_mats.back().type() == CV_32FC1)
		{
			for (int32 j = 0; j < m_mats.back().rows && j < h; ++j)
			{
				for (int32 k = 0; k < m_mats.back().cols && k < w; ++k)
				{
					float& c1 = m_mats.back().at<float>(j, k);
					if (c1 < 0)
					{
						c1 = 0;
					}
				}
			}
		}
		normalize(m_mats.back(), m_mats.back(), 0, 255, cv::NORM_MINMAX);
	}
	else
	{
		m_mats[it - m_names.begin()] = mat;
	}
}

void cvshowEX::RemoveShow(std::string name)
{
}

void cvshowEX::Render()
{
	int32 needdraw = 0;
	for (int32 i = 0; i < m_ischecks.size(); ++i)
	{
		if (m_ischecks[i])
		{
			needdraw++;
		}
	}
	w = 0;
	h = 0;
	for (int32 i = 0; i < m_mats.size(); ++i)
	{
		if (m_mats[i].rows > h)
		{
			h = m_mats[i].rows;
		}
		if (m_mats[i].cols > w)
		{
			w = m_mats[i].cols;
		}
	}
	if (w > 1700)
	{
		w = 1700;
	}
	if (h > 1150)
	{
		h = 1150;
	}
	if (h < 800)
	{
		h = 800;
	}
	m_sizeimg.create(h, w + 200, CV_8UC3);
	double scale = 1.0 / needdraw;
	m_sizeimg = cv::Scalar(0, 0, 0);
	m_sizeimg.convertTo(m_sizeimg, CV_32FC3);
	for (int32 i = 0; i < m_mats.size(); ++i)
	{
		if (m_ischecks[i])
		{
			cv::Mat drawimg = m_mats[i];
			if (drawimg.type() == CV_8UC3)
			{
				for (int32 j = 0; j < drawimg.rows && j < h; ++j)
				{
					for (int32 k = 0; k < drawimg.cols && k < w; ++k)
					{
						cv::Vec3b c1 = drawimg.at<cv::Vec3b>(j, k);
						cv::Vec3f& c2 = m_sizeimg.at<cv::Vec3f>(j, k);
						c2[0] += scale * c1[0];
						c2[1] += scale * c1[1];
						c2[2] += scale * c1[2];
					}
				}
			}
			else if (drawimg.type() == CV_8UC1)
			{
				for (int32 j = 0; j < drawimg.rows && j < h; ++j)
				{
					for (int32 k = 0; k < drawimg.cols && k < w; ++k)
					{
						uchar c1 = drawimg.at<uchar>(j, k);
						cv::Vec3f& c2 = m_sizeimg.at<cv::Vec3f>(j, k);
						c2[0] += scale * c1;
						c2[1] += scale * c1;
						c2[2] += scale * c1;
					}
				}
			}
			else if (drawimg.type() == CV_32FC1)
			{
				for (int32 j = 0; j < drawimg.rows && j < h; ++j)
				{
					for (int32 k = 0; k < drawimg.cols && k < w; ++k)
					{
						float c1 = drawimg.at<float>(j, k);
						cv::Vec3f& c2 = m_sizeimg.at<cv::Vec3f>(j, k);
						c2[0] += scale * c1;
						c2[1] += scale * c1;
						c2[2] += scale * c1;
					}
				}
			}
		}
	}
	normalize(m_sizeimg, m_sizeimg, 0, 255, cv::NORM_MINMAX);
	m_sizeimg.convertTo(m_sizeimg, CV_8UC3);
	cv::imshow("cvshowEX", m_sizeimg);
	for (int32 i = 0; i < m_hwnd_checks.size(); ++i)
	{
		SetWindowPos(m_hwnd_checks[i], 0, w, 20 * i, 200, 20,
					 SWP_DRAWFRAME | SWP_SHOWWINDOW);
	}
}

void cvshowEX::Init()
{
	m_num_check = 0;
	w = 1600;
	h = 1000;
	m_sizeimg.create(h, w + 200, CV_8UC3);
	cv::imshow("cvshowEX", m_sizeimg);
	g_hwnd2 = (HWND)cvGetWindowHandle("cvshowEX");
	g_ori_wproc2 = GetWindowLongPtr(g_hwnd2, GWLP_WNDPROC);
	SetWindowLongPtr(g_hwnd2, GWLP_WNDPROC, g_my_wproc2);
}
