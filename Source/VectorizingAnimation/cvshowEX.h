#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <vector>
#include <string>
#include <windows.h>

typedef std::vector<cv::Mat> Mats;
typedef std::vector<std::string> strings;
typedef std::vector<HWND> HWNDs;
typedef std::vector<char> chars;

class cvshowEX
{
public:
	HWND  m_main;
	HWNDs m_hwnd_checks;
	Mats  m_mats;
	cv::Mat m_sizeimg;
	chars m_ischecks;
	strings m_names;
	int m_num_check;
	cvshowEX(void);
	~cvshowEX(void);
	void Init();
	void AddShow(std::string name, cv::Mat mat);
	void RemoveShow(std::string name);
	void Render();
	int w, h;
};

extern cvshowEX g_cvshowEX;