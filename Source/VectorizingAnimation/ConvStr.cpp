#include "VectorizingAnimation.h"
#include "ConvStr.h"
#include <clocale>
#pragma warning(disable : 4996) // use sprintf


void ConvStr::CharToWchar(wchar_t* unicode, const char* ansi)
{
	mbstowcs(unicode, ansi, INT_MAX);
}
void ConvStr::WcharToChar(const wchar_t* unicode, char* ansi)
{
	setlocale(LC_ALL, "cht");
	wcstombs(ansi, unicode, INT_MAX);
}

std::wstring ConvStr::GetWstr(int number)
{
	wchar_t data[32];
	swprintf(data, L"%d", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr(unsigned int number)
{
	wchar_t data[32];
	swprintf(data, L"%d", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr(long long number)
{
	wchar_t data[32];
	swprintf(data, L"%ld", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr(float number)
{
	wchar_t data[32];
	swprintf(data, L"%f", number);
	return std::wstring(data);
}

std::wstring ConvStr::GetWstr(double number)
{
	wchar_t data[32];
	swprintf(data, L"%lf", number);
	return std::wstring(data);
}

std::string ConvStr::GetStr(int number)
{
	char data[32];
	sprintf(data, "%d", number);
	return std::string(data);
}

std::string ConvStr::GetStr(unsigned int number)
{
	char data[32];
	sprintf(data, "%d", number);
	return std::string(data);
}

std::string ConvStr::GetStr(long long number)
{
	char data[32];
	sprintf(data, "%ld", number);
	return std::string(data);
}

std::string ConvStr::GetStr(float number)
{
	char data[32];
	sprintf(data, "%f", number);
	return std::string(data);
}

std::string ConvStr::GetStr(double number)
{
	char data[32];
	sprintf(data, "%lf", number);
	return std::string(data);
}
std::wstring ConvStr::GetWstr(std::wstring unicode)
{
	return std::wstring(unicode);
}

std::wstring ConvStr::GetWstr(std::string ansi)
{
	std::vector<wchar_t> unicode;
	unicode.resize(ansi.length() + 1);
	CharToWchar(&unicode[0], &ansi[0]);
	return std::wstring(&unicode[0]);
}

std::string ConvStr::GetStr(std::wstring unicode)
{
	std::vector<char> ansi;
	ansi.resize(unicode.length() * 2 + 1);
	WcharToChar(&unicode[0], &ansi[0]);
	return std::string(&ansi[0]);
}

std::string ConvStr::GetStr(std::string ansi)
{
	return std::string(ansi);
}
