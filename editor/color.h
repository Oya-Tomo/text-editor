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

string coloringText(string text, int mode);

string coloringHtml(string text);

#endif // !COLOR_H
