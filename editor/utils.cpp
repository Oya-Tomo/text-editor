#include "utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <WinUser.h>

using namespace std;

vector<string> split(string text, string target)
{
	vector<string> lines{};
	size_t pos;

	while ((pos = text.find(target)) != string::npos)
	{
		lines.push_back(text.substr(0, pos));
		text.erase(0, pos + target.length());
	}
	lines.push_back(text);

	return lines;
}


void setCursorPosition(int x, int y) {
	COORD pos = { x, y };
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(consoleHandle, pos);
}

void getConsoleSize(int* width, int* height) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	*width = info.srWindow.Right - info.srWindow.Left + 1;
	*height = info.srWindow.Bottom - info.srWindow.Top + 1;
}

void setClipBoardText(string text)
{
	HWND hwnd = GetClipboardOwner();
	HANDLE hmem = GlobalAlloc(GHND, (strlen(text.c_str()) + 1));
	if (hmem) {
		LPSTR lpMem = (LPSTR)GlobalLock(hmem);
		memcpy(lpMem, text.c_str(), (strlen(text.c_str()) + 1));
		GlobalUnlock(hmem);
		if (OpenClipboard(hwnd)) {
			if (EmptyClipboard() && text != "") {
				SetClipboardData(CF_TEXT, hmem);
				CloseClipboard();
				SetConsoleTitleA("Copied text !");
			}
		}
		else {
			SetConsoleTitleA("Copy failed !!");
		}
	}
}

string getClipBoardText()
{
	HWND hwnd = GetClipboardOwner();
	if (!OpenClipboard(hwnd)) {
		SetConsoleTitleA("Copy failed !!");
		return "";
	}
	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) {
		SetConsoleTitleA("Copy failed !!");
		return "";
	}
	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr) {
		SetConsoleTitleA("Copy failed !!");
		return "";
	}
	string text(pszText);
	GlobalUnlock(hData);
	CloseClipboard();
	return text;
}

bool checkFileExists(const std::string& str)
{
	std::ifstream ifs(str);
	return ifs.is_open();
}

std::string UTF8toSjis(std::string strUTF8)
{
	const int unicodeSize = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeSize];
	MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, pUnicode, unicodeSize);

	const int sjisSize = WideCharToMultiByte(CP_SJIS, 0, pUnicode, -1, NULL, 0, NULL, NULL);
	char* pSjis = new char[sjisSize];
	WideCharToMultiByte(CP_SJIS, 0, pUnicode, -1, pSjis, sjisSize, NULL, NULL);

	string strSjis(pSjis);

	delete[] pSjis;
	pSjis = NULL;
	delete[] pUnicode;
	pUnicode = NULL;

	return strSjis;
}

std::string SjistoUTF8(std::string strSjis)
{
	const int unicodeSize = MultiByteToWideChar(CP_SJIS, 0, strSjis.c_str(), -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeSize];
	MultiByteToWideChar(CP_SJIS, 0, strSjis.c_str(), -1, pUnicode, unicodeSize);

	const int utf8Size = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, NULL, NULL);
	char* pUtf8 = new char[utf8Size];
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUtf8, utf8Size, NULL, NULL);

	string strUtf8(pUtf8);

	delete[] pUtf8;
	pUtf8 = NULL;
	delete[] pUnicode;
	pUnicode = NULL;

	return strUtf8;
}

vector<MatchData> extractMatchesAll(string text, regex rgx, int count)
{
	string target = text;
	vector<MatchData> results;
	int lastPos = 0;
	count++;

	while (true)
	{
		smatch searchResult;
		smatch partResult;
		string part;
		MatchData match;

		if (regex_search(target, searchResult, rgx)) {
			part = target.substr(searchResult.position(), searchResult.length());

			match.pos = lastPos + searchResult.position();
			match.length = searchResult.length();

			lastPos += searchResult.position() + searchResult.length();
			target.erase(0, searchResult.position() + searchResult.length());

			if (regex_match(part, partResult, rgx)) {
				for (int i = 0; i < count; i++) {
					match.data.push_back(partResult[i].str());
				}
				results.push_back(match);
			}
		}
		else {
			break;
		}
	}
	return results;
}