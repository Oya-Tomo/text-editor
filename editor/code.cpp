#include <string>
#include <vector>
#include <Windows.h>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <regex>
#include "code.h"
#include "utils.h"
#include "color.h"

using namespace std;

static int width;
static int height;

Code::Code()
{
	this->text = vector<string>({ "" });
	this->x = 0;
	this->y = 0;
	this->_x = 0;
	this->renderAll = false;
	//this->charCounts = { 0 };
	this->colorMode = NORMAL_MODE;

	this->top = 0;

	getConsoleSize(&width, &height);
}

void Code::setText(string content)
{
	this->text = split(content, "\n");

	//vector<int> charCount = {};
	//for (int i = 0; i < this->text.size(); i++) {
		//charCount.push_back(this->text[i].size());
	//}
	//this->charCounts = charCount;
}

vector<string> Code::getText()
{
	return this->text;
}

void Code::setColorMode(int mode)
{
	this->colorMode = mode;
}

void Code::renderViewCode()
{
	std::cout << "\x1b[?25l";
	system("cls");

	int lineCount = 0;

	for (int i = this->top; i < height + this->top; i++) {
		int currentLineCount = lineCount;
		if (i == this->text.size()) {
			break;
		}
		lineCount += this->text[i].size() / width;
		lineCount += 1;
		if (lineCount > height) {
			break;
		}
		cout << "\x1b[" << currentLineCount + 1 << ";1H";
		cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
	}

	std::cout << "\x1b[?25h";
	this->renderAll = false;
}

void Code::renderOneLineCode()
{
	std::cout << "\x1b[?25l";
	int lineCount = 0;
	int lines = 0;

	for (int i = this->top; i < height + this->top; i++) {
		int currentLineCount = lineCount;
		if (i == this->text.size()) {
			break;
		}
		lines = this->text[i].size() / width + 1;
		lineCount += lines;
		if (lineCount > height) {
			break;
		}
		if (i == this->y) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << "\x1b[" << lines << "M";
			cout << "\x1b[" << lines << "L";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
			break;
		}
	}
	
	std::cout << "\x1b[?25h";
}

void Code::renderAroundCode()
{
	std::cout << "\x1b[?25l";
	int lineCount = 0;
	int lines = 0;

	for (int i = this->top; i < height + this->top; i++) {
		int currentLineCount = lineCount;
		if (i == this->text.size()) {
			break;
		}
		lines = this->text[i].size() / width + 1;
		lineCount += lines;
		if (lineCount > height) {
			break;
		}
		if (i == this->y - 1) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << "\x1b[" << lines << "M";
			cout << "\x1b[" << lines << "L";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
		}
		else if (i == this->y) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << "\x1b[" << lines << "M";
			cout << "\x1b[" << lines << "L";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
		}
		else if (i == this->y + 1) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << "\x1b[" << lines << "M";
			cout << "\x1b[" << lines << "L";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
			break;
		}
	}

	std::cout << "\x1b[?25h";
}

void Code::renderCode(string keyEvent)
{
	getConsoleSize(&width, &height);

	if (keyEvent == "<[up]>" || keyEvent == "<[ctrl-k]>") {
		int oldTop = this->top;
		this->moveUp();
		this->scrollView();
		int newTop = this->top;
		if (this->renderAll) {
			this->renderViewCode();
		}
		else if (oldTop != newTop) {
			this->renderScrollUpView(oldTop, newTop);
		}
	}
	else if (keyEvent == "<[down]>" || keyEvent == "<[ctrl-j]>") {
		int oldTop = this->top;
		this->moveDown();
		this->scrollView();
		int newTop = this->top;
		if (this->renderAll) {
			this->renderViewCode();
		}
		else if (oldTop != newTop) {
			this->renderScrollDownView(oldTop, newTop);
		}
	}
	else if (keyEvent == "<[left]>" || keyEvent == "<[ctrl-h]>") {
		int oldTop = this->top;
		this->moveLeft();
		this->scrollView();
		int newTop = this->top;
		if (this->renderAll) {
			this->renderViewCode();
		}
		else if (oldTop != newTop) {
			this->renderScrollUpView(oldTop, newTop);
		}
	}
	else if (keyEvent == "<[right]>" || keyEvent == "<[ctrl-l]>") {
		int oldTop = this->top;
		this->moveRight();
		this->scrollView();
		int newTop = this->top;
		if (this->renderAll) {
			this->renderViewCode();
		}
		else if (oldTop != newTop) {
			this->renderScrollDownView(oldTop, newTop);
		}
	}
	else if (keyEvent == "<[enter]>") {
		this->pressEnter();
		this->scrollView();
		this->renderViewCode();
	}
	else if (keyEvent == "<[shift-up]>") {
		int oldTop = this->top;
		this->moveUpWithPool();
		this->scrollView();
		int newTop = this->top;
		if (oldTop != newTop) {
			this->renderViewCode();
		}
		else {
			this->renderAroundCode();
		}
		this->renderAll = true;
	}
	else if (keyEvent == "<[shift-down]>") {
		int oldTop = this->top;
		this->moveDownWithPool();
		this->scrollView();
		int newTop = this->top;
		if (oldTop != newTop) {
			this->renderViewCode();
		}
		else {
			this->renderAroundCode();
		}
		this->renderAll = true;
	}
	else if (keyEvent == "<[shift-left]>") {
		int oldTop = this->top;
		this->moveLeftWithPool();
		this->scrollView();
		int newTop = this->top;
		if (oldTop != newTop) {
			this->renderViewCode();
		}
		else {
			this->renderAroundCode();
		}
		this->renderAll = true;
	}
	else if (keyEvent == "<[shift-right]>") {
		int oldTop = this->top;
		this->moveRightWithPool();
		this->scrollView();
		int newTop = this->top;
		if (oldTop != newTop) {
			this->renderViewCode();
		}
		else {
			this->renderAroundCode();
		}
		this->renderAll = true;
	}
	else if (keyEvent == "<[tab]>") {
		this->pressTab();
		this->scrollView();
		if (this->renderAll) {
			this->renderViewCode();
		}
		else {
			this->renderOneLineCode();
		}
	}
	else if (keyEvent == "<[back]>") {
		if (this->x == 0) {
			this->pressBack();
			this->scrollView();
			if (this->renderAll) {
				this->renderViewCode();
			}
		}
		else {
			this->pressBack();
			if (this->renderAll) {
				this->renderViewCode();
			}
			else {
				this->renderOneLineCode();
			}
		}
	}
	else if (keyEvent == "<[delete]>" or keyEvent == "<[shift-delete]>") {
		if (this->x == this->text[this->y].length()) {
			this->pressDelete();
			this->scrollView();
			if (this->renderAll) {
				this->renderViewCode();
			}
		}
		else {
			this->pressDelete();
			if (this->renderAll) {
				this->renderViewCode();
			}
			else {
				this->renderOneLineCode();
			}
		}
	}
	else if (keyEvent == "<[ctrl-c]>") {
		setClipBoardText(this->getRangeText());
	}
	else if (keyEvent == "<[ctrl-x]>") {
		setClipBoardText(this->getRangeText());
		this->deleteRangeText();
		this->poolPosition();
		this->scrollView();
		this->renderViewCode();
	}
	else if (keyEvent == "<[ctrl-t]>") {
		this->insertIndent();
		this->renderViewCode();
		if (this->x != this->poolingX || this->y != this->poolingY) {
			this->renderAll = true;
		}
	}
	else if (keyEvent == "<[ctrl-e]>") {
		this->insertLine();
		this->renderViewCode();
	}
	else if (keyEvent == "<[ctrl-r]>") {
		this->scrollView();
		this->renderViewCode();
	}
	else if (keyEvent == "<[ctrl-v]>") {
		vector<string> pasteLines = split(getClipBoardText(), "\r");

		for (int i = 0; i < pasteLines.size(); i++) {
			this->insertString(pasteLines[i]);
			if (i != pasteLines.size() - 1) {
				this->pressEnter();
			}
		}
		this->scrollView();
		if (this->renderAll) {
			this->renderViewCode();
		}
		else {
			this->renderOneLineCode();
		}
	}
	else {
		vector<string> insertText = split(keyEvent, "\r");

		for (int i = 0; i < insertText.size(); i++) {
			this->insertString(insertText[i]);
			if (i != insertText.size() - 1) {
				this->pressEnter();
			}
		}
		this->scrollView();
		if (this->renderAll) {
			this->renderViewCode();
		}
		else {
			this->renderOneLineCode();
		}
	}
	this->setRelativeCursorPos();
}

void Code::renderScrollUpView(int oldTop, int newTop)
{
	int diff = 0;
	for (int i = newTop; i < oldTop; i++) {
		diff += this->text[i].size() / width;
		diff += 1;
	}
	cout << "\x1b[?25l";
	cout << "\x1b[" << diff << "T";

	int lineCount = 0;

	for (int i = this->top; i < height + this->top; i++) {
		int currentLineCount = lineCount;
		if (i == this->text.size()) {
			break;
		}
		lineCount += this->text[i].size() / width;
		lineCount += 1;
		if (lineCount > height) {
			break;
		}
		if (i < oldTop) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
		}
	}
	
	cout << "\x1b[?25h";
}

void Code::renderScrollDownView(int oldTop, int newTop)
{
	int diff = 0;
	for (int i = oldTop; i < newTop; i++) {
		diff += this->text[i].size() / width;
		diff += 1;
	}
	cout << "\x1b[?25l";
	cout << "\x1b[" << diff << "S";

	int lineCount = 0;

	for (int i = this->top; i < height + this->top; i++) {
		int currentLineCount = lineCount;
		if (i == this->text.size()) {
			break;
		}
		lineCount += this->text[i].size() / width;
		lineCount += 1;
		if (lineCount > height) {
			break;
		}
		if (i > this->getBottomIndex(oldTop)) {
			cout << "\x1b[" << currentLineCount + 1 << ";1H";
			cout << coloringText(this->text[i], this->colorMode, i, this->x, this->y, this->poolingX, this->poolingY) + " ";
		}
	}
	
	cout << "\x1b[?25h";
}

int Code::getTopIndex(int bottomIndex)
{
	int lineCount = 0;
	int lastIndex = 0;
	for (int i = bottomIndex; i > bottomIndex - height; i--) {
		if (i < 0) {
			break;
		}
		lineCount += this->text[i].size() / width;
		lineCount += 1;
		if (lineCount > height) {
			break;
		}
		lastIndex = i;
	}
	return lastIndex;
}

int Code::getBottomIndex(int topIndex)
{
	int lineCount = 0;
	int lastIndex = 0;
	for (int i = topIndex; i < height + topIndex; i++) {
		if (i >= this->text.size()) {
			break;
		}
		lineCount += this->text[i].size() / width;
		lineCount += 1;
		if (lineCount > height) {
			break;
		}
		lastIndex = i;
	}
	return lastIndex;
}

void Code::scrollView()
{
	int oldTop = this->top;

	if (this->top > this->y) {
		this->top = this->y;
	}
	else if (this->getBottomIndex(this->top) < this->y) {
		this->top = this->getTopIndex(this->y);
	}
}

void Code::setRelativeCursorPos()
{
	//std::cout << "\x1b[" << this->y - this->top + 1 << ";" << this->x + 1 << "H";

	int column = 0;
	int lineCount = 0;
	for (int i = this->top; i <= this->y; i++) {
		if (i < this->y) {
			if (this->text[i].size() == 0) {
				lineCount += 1;
			}
			else {
				lineCount += this->text[i].size() / width;
				lineCount += 1;
			}
		}
		else {
			if (this->x == 0) {
				lineCount += 1;
			}
			else {
				lineCount += this->x / width;
				lineCount += 1;
				column = this->x % width + 1;
			}
		}
	}
	cout << "\x1b[" << lineCount << ";" << column << "H";
	std::cout << "\x1b[?25h";
}

void Code::moveLeft()
{
	if (this->x == 0) {
		if (this->y > 0) {
			this->y--;
			this->x = this->text[this->y].length();
		}
	}
	else {
		if (this->x == 1) {
			this->x = 0;
		}
		else if ((bool)IsDBCSLeadByte(this->text[this->y][this->x - 2]) == 1) {
			this->x -= 2;
		}
		else {
			this->x--;
		}
	}
	this->poolXPosition();
	this->poolPosition();
}

void Code::moveRight()
{
	if (this->x == this->text[this->y].length()) {
		if (this->y < this->text.size() - 1) {
			this->y++;
			this->x = 0;
			this->poolXPosition();
		}
	}
	else {
		if ((bool)IsDBCSLeadByte(this->text[this->y][this->x]) == 1) {
			this->x += 2;
		}
		else {
			this->x++;
		}
	}
	this->poolXPosition();
	this->poolPosition();
}

void Code::moveUp()
{
	if (this->y == 0) {
		this->x = 0;
	}
	else {
		this->y--;
		if (this->_x <= this->text[this->y].length()) {
			this->x = this->_x;
		}
		else {
			this->x = this->text[this->y].length();
		}
	}
	this->poolPosition();
}

void Code::moveDown()
{
	if (this->y == this->text.size() - 1) {
		this->x = this->text[this->y].length();
	}
	else {
		this->y++;
		if (this->_x <= this->text[this->y].length()) {
			this->x = this->_x;
		}
		else {
			this->x = this->text[this->y].length();
		}
	}
	this->poolPosition();
}

void Code::moveLeftWithPool()
{
	if (this->x == 0) {
		if (this->y > 0) {
			this->y--;
			this->x = this->text[this->y].length();
		}
	}
	else {
		if (this->x == 1) {
			this->x = 0;
		}
		else if ((bool)IsDBCSLeadByte(this->text[this->y][this->x - 2]) == 1) {
			this->x -= 2;
		}
		else {
			this->x--;
		}
	}
	this->poolXPosition();
	this->renderAll = true;
}

void Code::moveRightWithPool()
{
	if (this->x == this->text[this->y].length()) {
		if (this->y < this->text.size() - 1) {
			this->y++;
			this->x = 0;
			this->poolXPosition();
		}
	}
	else {
		if ((bool)IsDBCSLeadByte(this->text[this->y][this->x]) == 1) {
			this->x += 2;
		}
		else {
			this->x++;
		}
	}
	this->poolXPosition();
	this->renderAll = true;
}

void Code::moveUpWithPool()
{
	if (this->y == 0) {
		this->x = 0;
	}
	else {
		this->y--;
		if (this->_x <= this->text[this->y].length()) {
			this->x = this->_x;
		}
		else {
			this->x = this->text[this->y].length();
		}
	}
	this->renderAll = true;
}

void Code::moveDownWithPool()
{
	if (this->y == this->text.size() - 1) {
		this->x = this->text[this->y].length();
	}
	else {
		this->y++;
		if (this->_x <= this->text[this->y].length()) {
			this->x = this->_x;
		}
		else {
			this->x = this->text[this->y].length();
		}
	}
	this->renderAll = true;
}


void Code::insertString(string content)
{
	string origin_txt = content;

	int oldLines = this->text[this->y].size() / width;
	while (!origin_txt.empty())
	{
		if (IsDBCSLeadByte(origin_txt[0]) == 0) {
			this->text[this->y].insert(this->x, origin_txt.substr(0, 1));
			origin_txt.erase(0, 1);
			this->x++;
		}
		else {
			this->text[this->y].insert(this->x, origin_txt.substr(0, 2));
			origin_txt.erase(0, 2);
			this->x += 2;
		}
	}
	int newLines = this->text[this->y].size() / width;
	if (newLines != oldLines) {
		this->renderAll = true;
	}
	this->poolXPosition();
	this->poolPosition();
	//this->charCounts[this->y] = this->text[this->y].size();
}

void Code::pressEnter()
{
	if (this->x == 0) {
		this->text.insert(this->text.begin() + this->y, "");
		this->y++;
		this->x = 0;

		//this->charCounts.insert(this->charCounts.begin() + this->y - 1, 0);
	}
	else if (this->x == this->text[this->y].length()) {
		this->y++;
		this->text.insert(this->text.begin() + this->y, "");
		this->x = 0;

		//this->charCounts.insert(this->charCounts.begin() + this->y, 0);
	}
	else {
		string substr = this->text[this->y].substr(this->x);
		this->text[this->y].erase(this->x);
		this->y++;
		this->text.insert(this->text.begin() + this->y, substr);
		this->x = 0;

		//this->charCounts[this->y - 1] = this->text[this->y - 1].length();
		//this->charCounts.insert(this->charCounts.begin() + this->y, substr.length());
	}
	this->renderAll = true;
	this->poolXPosition();
	this->poolPosition();
}

void Code::pressBack()
{
	if (this->x == this->poolingX && this->y == this->poolingY) {
		if (this->x == 0) {
			if (this->y > 0) {
				this->x = this->text[this->y - 1].length();
				this->text[this->y - 1] += this->text[this->y];
				this->text.erase(this->text.begin() + this->y);
				this->y--;

				this->renderAll = true;

				//this->charCounts[this->y] = this->text[this->y].length();
				//this->charCounts.erase(this->charCounts.begin() + this->y + 1);
			}
		}
		else {
			int oldLines = this->text[this->y].size() / width;
			if (this->x == 1) {
				this->text[this->y].erase(0, 1);
				this->x--;
			}
			else if ((bool)IsDBCSLeadByte(this->text[this->y][this->x - 2]) == 1) {
				this->text[this->y].erase(this->x - 2, 2);
				this->x -= 2;
			}
			else {
				this->text[this->y].erase(this->x - 1, 1);
				this->x--;
			}
			//this->charCounts[this->y] = this->text[this->y].length();
			int newLines = this->text[this->y].size() / width;
			if (newLines != oldLines) {
				this->renderAll = true;
			}
		}
	}
	else {
		this->deleteRangeText();
	}
	this->poolXPosition();
	this->poolPosition();
}

void Code::pressDelete()
{
	if (this->x == this->poolingX && this->y == this->poolingY) {
		if (this->x == this->text[this->y].length()) {
			if (this->y != this->text.size() - 1) {
				this->text[this->y] += this->text[this->y + 1];
				this->text.erase(this->text.begin() + this->y + 1, this->text.begin() + this->y + 2);

				//this->charCounts[this->y] = this->text[this->y].size();
				//this->charCounts.erase(this->charCounts.begin() + this->y + 1);
				this->renderAll = true;
			}
		}
		else {
			int oldLines = this->text[this->y].size() / width;
			if ((bool)IsDBCSLeadByte(this->text[this->y][this->x]) == 1) {
				this->text[this->y].erase(this->text[this->y].begin() + this->x, this->text[this->y].begin() + this->x + 2);
			}
			else {
				this->text[this->y].erase(this->text[this->y].begin() + this->x, this->text[this->y].begin() + this->x + 1);
			}
			//this->charCounts[this->y] = this->text[this->y].size();
			int newLines = this->text[this->y].size() / width;
			if (newLines != oldLines) {
				this->renderAll = true;
			}
		}
	}
	else {
		this->deleteRangeText();
	}
	this->poolXPosition();
	this->poolPosition();
}

void Code::pressTab()
{
	this->insertString("    ");
}

void Code::insertIndent()
{
	if (this->y <= this->poolingY) {
		for (int i = this->y; i <= this->poolingY; i++) {
			this->text[i] = "    " + this->text[i];
			//this->charCounts[i] += 4;
		}
	}
	else {
		for (int i = this->poolingY; i <= this->y; i++) {
			this->text[i] = "    " + this->text[i];
			//this->charCounts[i] += 4;
		}
	}
	this->x += 4;
	this->poolingX += 4;
	this->poolXPosition();
	this->renderAll = true;
}

void Code::insertLine()
{
	this->x = 0;
	this->y++;
	this->text.insert(this->text.begin() + this->y, "");
	//this->charCounts.insert(this->charCounts.begin() + this->y, 0);
	this->poolXPosition();
	this->poolPosition();
	this->renderAll = true;
}

string Code::getRangeText()
{
	if (this->x == this->poolingX && this->y == this->poolingY) {
		return this->text[this->y];
	}
	else if (this->y > this->poolingY) {
		string copyText = "";
		for (int i = this->poolingY; i <= this->y; i++) {
			if (i == this->poolingY) {
				copyText += this->text[i].substr(this->poolingX, this->text[i].length() - this->poolingX) + "\r";
			}
			else if (i == this->y) {
				copyText += this->text[i].substr(0, this->x);
			}
			else {
				copyText += this->text[i] + "\r";
			}
		}
		return copyText;
	}
	else if (this->y < this->poolingY) {
		string copyText = "";
		for (int i = this->y; i <= this->poolingY; i++) {
			if (i == this->y) {
				copyText += this->text[i].substr(this->x, this->text[i].length() - this->x) + "\r";
			}
			else if (i == this->poolingY) {
				copyText += this->text[i].substr(0, this->poolingX);
			}
			else {
				copyText += this->text[i] + "\r";
			}
		}
		return copyText;
	}
	else {
		if (this->x > this->poolingX) {
			return this->text[this->y].substr(this->poolingX, this->x - this->poolingX);
		}
		else {
			return this->text[this->y].substr(this->x, this->poolingX - this->x);
		}
	}
}

void Code::deleteRangeText()
{
	string rangeText = this->getRangeText();
	if (this->x == this->poolingX && this->y == this->poolingY) {
		if (this->y != this->text.size() - 1) {
			this->text.erase(this->text.begin() + this->y);
			if (this->text[this->y].size() < this->x) {
				this->x = this->text[this->y].size();
			}

			//this->charCounts.erase(this->charCounts.begin() + this->y);
		}
		else {
			if (this->y != 0) {
				this->text.erase(this->text.begin() + this->y);
				this->y--;
				if (this->text[this->y].size() < this->x) {
					this->x = this->text[this->y].size();
				}
			}
			else {
				this->text[0] = "";
				this->x = 0;
			}
		}
		this->poolXPosition();
		this->poolPosition();
	}
	else {
		if (this->y > this->poolingY) {
			this->x = this->poolingX;
			this->y = this->poolingY;
		}
		else if (this->y == this->poolingY) {
			if (this->x > this->poolingX) {
				this->x = this->poolingX;
			}
		}
		for (int i = 0; i < rangeText.length(); i++) {
			if (this->x == this->text[this->y].length()) {
				if (this->y != this->text.size() - 1) {
					this->text[this->y] += this->text[this->y + 1];
					this->text.erase(this->text.begin() + this->y + 1, this->text.begin() + this->y + 2);
				}
			}
			else {
				if ((bool)IsDBCSLeadByte(this->text[this->y][this->x]) == 1) {
					this->text[this->y].erase(this->text[this->y].begin() + this->x, this->text[this->y].begin() + this->x + 2);
				}
				else {
					this->text[this->y].erase(this->text[this->y].begin() + this->x, this->text[this->y].begin() + this->x + 1);
				}
			}
		}
	}
	this->renderAll = true;
}

void Code::poolXPosition()
{
	this->_x = this->x;
}

void Code::poolPosition()
{
	this->poolingX = this->x;
	this->poolingY = this->y;
}
