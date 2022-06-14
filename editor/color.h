#pragma once
#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <string>
#include <regex>

#define NORMAL_MODE 0
#define HTML_MODE 1
#define CPP_MODE 2

using namespace std;

string coloringText(string text, int mode, int line, int x1, int y1, int x2, int y2);

string coloringHTML(string text, int start, int end);


#endif // !COLOR_H
