#pragma once
#ifndef UTILS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> split(string text, string target);
void printPartial(int x, int y, string text);

void setCursorPosition(int x, int y);
void getConsoleSize(int* width, int* height);

#endif // !UTILS_H
