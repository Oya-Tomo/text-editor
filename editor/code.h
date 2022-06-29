#pragma once
#ifndef CODE_H
#define CODE_H

#include <string>
#include <vector>

using namespace std;

class Code
{
public:
	Code();
	void setText(string content);
	vector<string> getText();

	void renderCode(string keyEvent);
	void renderViewCode();
	void renderOneLineCode();
	void renderAroundCode();

	void setColorMode(int mode);

private:
	vector<string> text;
	int x;
	int y;
	int _x; // pool cursor x positon when you move cursor to right or left.
	//vector<int> charCounts;

	int poolingX;
	int poolingY;
	bool renderAll;

	int colorMode;

	int top;
	void renderScrollUpView(int oldTop, int newTop);
	void renderScrollDownView(int oldTop, int newTop);
	int getTopIndex(int bottomIndex);
	int getBottomIndex(int topIndex);
	void scrollView();
	void setRelativeCursorPos();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void moveLeftWithPool();
	void moveRightWithPool();
	void moveUpWithPool();
	void moveDownWithPool();

	void insertString(string content);
	void pressEnter();
	void pressBack();
	void pressDelete();
	void pressTab();

	void insertIndent();
	void insertLine();

	string getRangeText();
	void deleteRangeText();
	void poolXPosition();
	void poolPosition();
};

#endif // !CODE_H