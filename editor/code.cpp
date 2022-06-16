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

Code::Code()
{
	this->text = vector<string>({ "" });
	this->x = 0;
	this->y = 0;
	this->_x = 0;
	this->poolled = false;
	//this->charCounts = { 0 };
	this->colorMode = NORMAL_MODE;

	this->top = 0;
}

void Code::setText(string content)
{
	this->text = split(content, "\n");
}

vector<string> Code::getText()
{
	return this->text;
}

void Code::setColorMode(int mode)
{
	this->colorMode = mode;
}

vector<int> Code::getViewSize()
{
	int width = 0;
	int height = this->text.size();

	return vector<int>{width, height};
}

void Code::renderViewCode()
{
	int width, height;
	getConsoleSize(&width, &height);

	std::cout << "\x1b[?25l";
	system("cls");
	std::cout << "\x1b[1;1H";

	if (this->top + height <= this->text.size()) {
		for (int i = 0; i < height; i++) {
			int start = 0;
			int end = this->text[i + this->top].size();

			std::cout << coloringText(this->text[i + this->top], this->colorMode, i + this->top, this->x, this->y, this->poolingX, this->poolingY);
			
			if (i != height - 1) {
				std::cout << "\n";
			}
		}
	}
	else {
		int lines = this->text.size() - this->top;
		for (int i = 0; i < lines; i++) {
			std::cout << coloringText(this->text[i + this->top], this->colorMode, i + this->top, this->x, this->y, this->poolingX, this->poolingY);
			if (i != lines - 1) {
				std::cout << "\n";
			}
		}
	}

	std::cout << "\x1b[?25h";
}

void Code::renderOneLineCode()
{
	std::cout << "\x1b[?25l";
	std::cout << "\x1b[" << this->y - this->top + 1 << ";1H";
	std::cout << "\x1b[2K";
	std::cout << coloringText(this->text[this->y], this->colorMode, this->y, this->x, this->y, this->poolingX, this->poolingY);
	std::cout << "\x1b[?25h";
}

void Code::renderAroundCode()
{
	int width, height;
	getConsoleSize(&width, &height);

	std::cout << "\x1b[?25l";
	if (0 < this->y - this->top && this->y - 1 >= 0) {
		std::cout << "\x1b[" << this->y - this->top << ";1H";
		std::cout << coloringText(this->text[this->y - 1], this->colorMode, this->y - 1, this->x, this->y, this->poolingX, this->poolingY);
	}
	if (height - 1 > this->y - this->top && this->y + 1 < this->text.size()) {
		std::cout << "\x1b[" << this->y - this->top + 2 << ";1H";
		std::cout << coloringText(this->text[this->y + 1], this->colorMode, this->y + 1, this->x, this->y, this->poolingX, this->poolingY);
	}
	std::cout << "\x1b[" << this->y - this->top + 1 << ";1H";
	std::cout << coloringText(this->text[this->y], this->colorMode, this->y, this->x, this->y, this->poolingX, this->poolingY);

	std::cout << "\x1b[?25h";
}

void Code::renderCode(string keyEvent)
{
	ostringstream lineNum;
	lineNum << setw(4) << setfill(' ') << this->y + 1;

	if (keyEvent == "<[up]>" || keyEvent == "<[ctrl-k]>") {
		this->moveUp();
		this->scrollView();
		if (this->poolled) {
			this->renderViewCode();
			this->poolled = false;
		}
	}
	else if (keyEvent == "<[down]>" || keyEvent == "<[ctrl-j]>") {
		this->moveDown();
		this->scrollView();
		if (this->poolled) {
			this->renderViewCode();
			this->poolled = false;
		}
	}
	else if (keyEvent == "<[left]>" || keyEvent == "<[ctrl-h]>") {
		this->moveLeft();
		this->scrollView();
		if (this->poolled) {
			this->renderViewCode();
			this->poolled = false;
		}
	}
	else if (keyEvent == "<[right]>" || keyEvent == "<[ctrl-l]>") {
		this->moveRight();
		this->scrollView();
		if (this->poolled) {
			this->renderViewCode();
			this->poolled = false;
		}
	}
	else if (keyEvent == "<[enter]>") {
		this->pressEnter();
		this->scrollView();
		this->renderViewCode();
		this->poolled = false;
	}
	else if (keyEvent == "<[shift-up]>") {
		this->moveUpWithPool();
		this->scrollView();
		this->renderAroundCode();
		this->poolled = true;
	}
	else if (keyEvent == "<[shift-down]>") {
		this->moveDownWithPool();
		this->scrollView();
		this->renderAroundCode();
		this->poolled = true;
	}
	else if (keyEvent == "<[shift-left]>") {
		this->moveLeftWithPool();
		this->scrollView();
		this->renderAroundCode();
		this->poolled = true;
	}
	else if (keyEvent == "<[shift-right]>") {
		this->moveRightWithPool();
		this->scrollView();
		this->renderAroundCode();
		this->poolled = true;
	}
	else if (keyEvent == "<[tab]>") {
		this->pressTab();
		if (this->poolled) {
			this->renderViewCode();
		}
		else {
			this->renderOneLineCode();
		}
		this->poolled = false;
	}
	else if (keyEvent == "<[back]>") {
		if (this->x == 0) {
			this->pressBack();
			this->scrollView();
			this->renderViewCode();
		}
		else {
			this->pressBack();
			if (this->poolled) {
				this->renderViewCode();
			}
			else {
				this->renderOneLineCode();
			}
		}
		this->poolled = false;
	}
	else if (keyEvent == "<[delete]>" or keyEvent == "<[shift-delete]>") {
		if (this->x == this->text[this->y].length()) {
			this->pressDelete();
			this->scrollView();
			this->renderViewCode();
		}
		else {
			this->pressDelete();
			if (this->poolled) {
				this->renderViewCode();
			}
			else {
				this->renderOneLineCode();
			}
		}
		this->poolled = false;
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
	}
	else if (keyEvent == "<[ctrl-e]>") {
		this->insertLine();
		this->renderViewCode();
		this->poolled = false;
	}
	else {
		vector<vector<string>> splitedLines = { split(keyEvent, "\r"), split(keyEvent, "\n"), split(keyEvent, "\r\n") };
		vector<int> lineSize = vector<int>{(int)splitedLines[0].size(), (int)splitedLines[1].size(), (int)splitedLines[2].size()};
		vector<int>::iterator iter = max_element(lineSize.begin(), lineSize.end());
		int index = distance(lineSize.begin(), iter);

		if (splitedLines[index].size() == 1) {
			this->insertString(keyEvent);
			if (this->poolled) {
				this->renderViewCode();
			}
			else {
				this->renderOneLineCode();
			}
		}
		else {
			for (int i = 0; i < splitedLines[index].size(); i++) {
				this->insertString(splitedLines[index][i]);
				if (i != i - 1) {
					this->pressEnter();
				}
			}
			this->scrollView();
			this->renderViewCode();
		}
		this->poolled = false;
	}
	this->setRelativeCursorPos();
}

void Code::renderScrollUpView(int diff)
{
	std::cout << "\x1b[?25l";

	std::cout << "\x1b[" << diff << "T";
	for (int i = 0; i < diff; i++) {
		std::cout << "\x1b[" << i + 1 << ";1H";
		//std::cout << this->text[this->top + i];
		std::cout << coloringText(this->text[i + this->top], this->colorMode, i + this->top, this->x, this->y, this->poolingX, this->poolingY);
	}
	
	std::cout << "\x1b[?25h";
}

void Code::renderScrollDownView(int diff)
{
	int width, height;
	getConsoleSize(&width, &height);

	std::cout << "\x1b[?25l";

	std::cout << "\x1b[" << diff << "S";
	for (int i = 0; i < diff; i++) {
		std::cout << "\x1b[" << height - i << ";1H";
		std::cout << coloringText(this->text[this->top + height - 1 - i], this->colorMode, this->top + height - 1 - i, this->x, this->y, this->poolingX, this->poolingY);
	}
	
	std::cout << "\x1b[?25h";
}

void Code::scrollView()
{
	int width, height;
	getConsoleSize(&width, &height);
	int oldTop = this->top;

	if (this->top > this->y) {
		this->top = this->y;
		this->renderScrollUpView(oldTop - this->top);
	}
	else if (this->top + height <= this->y) {
		this->top = this->y - height + 1;
		this->renderScrollDownView(this->top - oldTop);
	}
}

void Code::setRelativeCursorPos()
{
	std::cout << "\x1b[" << this->y - this->top + 1 << ";" << this->x + 1 << "H";
	std::cout << "\x1b[?25h";
}

void Code::moveLeft()
{
	this->moveLeftWithPool();
	this->poolPosition();
}

void Code::moveRight()
{
	this->moveRightWithPool();
	this->poolPosition();
}

void Code::moveUp()
{
	this->moveUpWithPool();
	this->poolPosition();
}

void Code::moveDown()
{
	this->moveDownWithPool();
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
}


void Code::insertString(string content)
{
	string origin_txt = content;

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
	this->poolXPosition();
	this->poolPosition();
	//this->charCounts[this->y] = this->text[this->y].length();
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
				this->text.erase(this->text.begin() + this->y, this->text.begin() + this->y + 1);
				this->y--;

				//this->charCounts[this->y] = this->text[this->y].length();
				//this->charCounts.erase(this->charCounts.begin() + this->y + 1, this->charCounts.begin() + this->y + 2);
			}
		}
		else {
			if (this->x == 1) {
				this->text[this->y].erase(this->text[this->y].begin(), this->text[this->y].begin() + 1);
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
		}
	}
	else {
		for (int i = this->poolingY; i <= this->y; i++) {
			this->text[i] = "    " + this->text[i];
		}
	}
	this->x += 4;
	this->poolingX += 4;
	this->poolXPosition();
}

void Code::insertLine()
{
	this->x = 0;
	this->y++;
	this->text.insert(this->text.begin() + this->y, "");
	this->poolXPosition();
	this->poolPosition();
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
