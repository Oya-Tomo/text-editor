#pragma once
#ifndef UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

vector<string> split(string text, string target);

void setCursorPosition(int x, int y);
void getConsoleSize(int* width, int* height);

void setClipBoardText(string text);

bool checkFileExists(const std::string& str);

string UTF8toSjis(string srcUTF8);
string SjistoUTF8(string srcSjis);

class MatchData {
public:
	int pos;
	int length;
	vector<string> data;
};

vector<MatchData> extractMatchesAll(string text, regex rgx, int count);

#endif // !UTILS_H
