#include "utils.h"
#include <iostream>
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

void printPartial(int x, int y, string text)
{
	std::cout << "\033[?25l";
	setCursorPosition(x, y);
	cout << text;
	std::cout << "\033[?25h";
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