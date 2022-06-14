#include <regex>
#include <vector>
#include "color.h"
#include "utils.h"

static const regex htmlRegex1 = regex("(</?!? *)([a-z]*[A-Z]*[0-9]*)([^>]*)(>)");
static const regex htmlRegex2 = regex("([^ =\"]*)( *)(=)( *)(\"[\\S ]*?\")( *)");

string coloringText(string text, int mode, int line, int x1, int y1, int x2, int y2)
{
	string target = text;
	if (mode == HTML_MODE) {
		if (y1 == y2 && y1 == line) {
			if (x1 < x2) {
				target = coloringHTML(target, x1, x2);
			}
			else if (x1 > x2) {
				target = coloringHTML(target, x2, x1);
			}
			else if (x1 == x2) {
				target = coloringHTML(target, 0, 0);
			}
		}
		else if (y1 == line) {
			if (y1 < y2) {
				target = coloringHTML(target, x1, target.size());
			}
			else {
				target = coloringHTML(target, 0, x1);
			}
		}
		else if (y2 == line) {
			if (y1 < y2) {
				target = coloringHTML(target, 0, x2);
			}
			else {
				target = coloringHTML(target, x2, target.size());
			}
		}
		else if ((y1 < line && line < y2) || (y1 > line && line > y2)) {
			target = coloringHTML(target, 0, target.size());
		}
		else {
			target = coloringHTML(target, 0, 0);
		}
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

string coloringHTML(string text, int start, int end)
{
	int currentLength = 0;

	vector<MatchData> results = extractMatchesAll(text, htmlRegex1, 4);
	string resultText = "";
	vector<string> splitedText = {};

	for (int i1 = 0; i1 < results.size(); i1++) {
		splitedText.push_back(text.substr(currentLength, results[i1].pos - currentLength));
		splitedText.push_back(results[i1].data[1]);
		splitedText.push_back("\x1b[34m");
		splitedText.push_back(results[i1].data[2]);
		splitedText.push_back("\x1b[37m");

		int currentPartLength = 0;
		vector<MatchData> props = extractMatchesAll(results[i1].data[3], htmlRegex2, 6);
		for (int i2 = 0; i2 < props.size(); i2++) {
			splitedText.push_back(results[i1].data[3].substr(currentPartLength, props[i2].pos - currentPartLength));
			splitedText.push_back("\x1b[32m");
			splitedText.push_back(props[i2].data[1]);
			splitedText.push_back(props[i2].data[2]);
			splitedText.push_back("\x1b[37m");
			splitedText.push_back(props[i2].data[3]);
			splitedText.push_back(props[i2].data[4]);
			splitedText.push_back("\x1b[33m");
			splitedText.push_back(props[i2].data[5]);
			splitedText.push_back(props[i2].data[6]);
			splitedText.push_back("\x1b[37m");
			currentPartLength = props[i2].pos + props[i2].length;
		}
		splitedText.push_back(results[i1].data[3].substr(currentPartLength, results[i1].data[3].size() - currentPartLength));
		splitedText.push_back(results[i1].data[4]);
		currentLength = results[i1].pos + results[i1].length;
	}
	if (currentLength < text.length()) {
		splitedText.push_back(text.substr(currentLength));
	}

	currentLength = 0;
	int range = 0;
	bool started = false;
	bool ended = false;

	string htmlText = "";
	for (int i = 0; i < splitedText.size(); i++) {
		if (splitedText[i] == "") {
			continue;
		}
		else if (splitedText[i][0] == '\x1b') {
			htmlText += splitedText[i];
			continue;
		}
		else if (((currentLength <= start && start <= currentLength + splitedText[i].size())
			&& (currentLength <= end && end <= currentLength + splitedText[i].size())) && (!started && !ended)) {
			int addSize = splitedText[i].size();
			string addText = splitedText[i];
			addText = addText.insert(start - currentLength, "\x1b[7m");
			addText = addText.insert(end - currentLength + 4, "\x1b[0m");
			htmlText += addText;
			started = true;
			ended = true;
			currentLength += addSize;
		}
		else if (currentLength <= start && start <= currentLength + splitedText[i].size() && !started) {
			int addSize = splitedText[i].size();
			htmlText += splitedText[i].insert(start - currentLength, "\x1b[7m");
			started = true;
			currentLength += addSize;
		}
		else if (currentLength <= end && end <= currentLength + splitedText[i].size() && !ended) {
			int addSize = splitedText[i].size();
			htmlText += splitedText[i].insert(end - currentLength, "\x1b[0m");
			ended = true;
			currentLength += addSize;
		}
		else {
			currentLength += splitedText[i].size();
			htmlText += splitedText[i];
		}
	}

	return htmlText;
}

