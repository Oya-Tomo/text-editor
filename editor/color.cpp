#include "color.h"

string coloringText(string text, int mode, int line, int x1, int y1, int x2, int y2)
{
	string target = text;
	if (mode == HTML_MODE) {
		return target;
	}
	else {
		if (y1 == y2 && y1 == line) {
			if (x1 < x2) {
				target = target.insert(x1, "\x1b[7m");
				target = target.insert(x2 + 4, "\x1b[0m");
			}
			else if (x1 > x2) {
				target = target.insert(x2, "\x1b[7m");
				target = target.insert(x1 + 4, "\x1b[0m");
			}
		}
		else if (y1 == line) {
			if (y1 < y2) {
				target = target.insert(x1, "\x1b[7m") + "\x1b[0m";
			}
			else {
				target = "\x1b[7m" + target.insert(x1, "\x1b[0m");
			}
		}
		else if (y2 == line) {
			if (y1 < y2) {
				target = "\x1b[7m" + target.insert(x2, "\x1b[0m");
			}
			else {
				target = target.insert(x2, "\x1b[7m") + "\x1b[0m";
			}
		}
		else if ((y1 < line && line < y2) || (y1 > line && line > y2)) {
			target = "\x1b[7m" + target + "\x1b[0m";
		}
		return target;
	}
}
