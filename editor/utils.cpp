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

bool checkFileExists(const std::string& str)
{
	std::ifstream ifs(str);
	return ifs.is_open();
}

std::string UTF8toSjis(std::string srcUTF8)
{
	int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);
	int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	char* bufShiftJis = new char[lengthSJis];
	WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

	std::string strSJis(bufShiftJis);

	return strSJis;
}

std::string SjistoUTF8(std::string srcSjis)
{
	int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
	wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);
	int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

	char* bufUTF8 = new char[lengthUTF8];
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode - 1, bufUTF8, lengthUTF8, NULL, NULL);

	std::string strUTF8(bufUTF8);

	return strUTF8;
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
