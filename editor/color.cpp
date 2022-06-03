#include "color.h"

string coloringText(string text, int mode) {
	if (mode == HTML_MODE) {
		return coloringHtml(text);
	}
	else {
		return text;
	}
}

string coloringHtml(string text) {
	return std::regex_replace(text, std::regex("(</?!?)([a-z]*[A-Z]*[0-9]*)([^>]*)(>)"), "$1\033[34m$2\033[33m$3\033[m$4");
}